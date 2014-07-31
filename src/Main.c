#include <stdio.h>
#include <stdlib.h>
#include "Delaunay.h"
#include "Subdivision.h"
#include "ArcsPointsAndOffsets.h"
#include "PointArrayList.h"
#include "ImportGML.h"
#include "ExportGML.h"
#include "PointList.h"
#include "Edge.h"

#define DIR "../td3"
#define ARCS "/lines_out.txt"
#define POINTS "/points_out.txt"

int edgeIsPartOfRing(struct Edge *test, struct Edge *fromOrigin);

int main() {
	printf("Reading data...");
	// IMPORT COORDINATES
	struct ArcsPointsAndOffsets *importedStuff;
	importedStuff = importGML_importGML(DIR ARCS, DIR POINTS);
	
	printf("done\n");
	
	printf("Triangulating points...");

	// Maintain a list of points to triangulate
	struct PointList *triPoints = (struct PointList*) malloc(sizeof(struct PointList));
	int triPointsSize;
	triPointsSize = 0;
	struct PointList *triIterator = triPoints;
	
	// add all constraint points and unique arc endpoints to the triangulation list
	int i;
	struct PointArrayList *arrayListIterator;
	arrayListIterator = importedStuff->points;
	// copy the first point to initialize the list
	triIterator->point = arrayListLiterator->points[0];
	triPointsSize++;
	// copy all remaining points from points list
	while (arrayListIterator) {
		for (triPointsSize = 1; i < arrayListIterator->numPoints; i++) {
			// make next list entry
			triIterator->next = (struct PointList*) malloc(sizeof(struct PointList));
			// advance the list iterator
			triIterator = triIterator->next;
			// copy pointers
			triIterator->point = arrayListIterator->points[i];
			// increase the count
			triPointsSize++;
		}
		arrayListIterator = arrayListIterator->next;
	}
	// copy unique endpoints from arcs list
	arrayListIterator = importedStuff->arcs;
	while (arrayListIterator) {
		struct Point *front;
		front = &arrayListIterator->points[0];
		struct Point *end;
		end = &arrayListIterator->points[arrayListIterator->numPoints-1];
		// test if these points are already in the list (O(n))
		struct PointList *dupCheckIter = triPoints;
		// if they are in the list, do not insert them again
		int shouldInsertFront;
		int shouldInsertEnd;
		shouldInsertFront = 1;
		shouldInsertEnd = 1;
		while (dupCheckIter) {
			if (shouldInsertFront && dupCheckIter->point.x == front->x && dupCheckIter->point.y == front->y) {
				// front is a duplicate
				shouldInsertFront = 0;
			}
			if (shouldInsertEnd && dupCheckIter->point.x == end->x && dupCheckIter->point.y == end->y) {
				shouldInsertEnd = 0;
			}
			if (!shouldInsertFront && !shouldInsertEnd) {
				break;
			}
			dupCheckIter = dupCheckIter->next;
		}
		if (shouldInsertFront) {
			triIterator->next = (struct PointList*) malloc(sizeof(struct PointList));
			triIterator = triIterator->next;
			triIterator->point = *front;
			triPointsSize++;
		}
		if (shouldInsertEnd) {
			triIterator->next = (struct PointList*) malloc(sizeof(struct PointList));
			triIterator = triIterator->next;
			triIterator->point = *end;
			triPointsSize++;
		}
		arrayListIterator = arrayListIterator->next;
	}

	// TRIANGULATE
	struct Subdivision *triangulation;
	triangulation = delaunay_triangulate(triPoints, triPointsSize);

	printf("done\n");
	// SIMPLIFY

	struct PointArrayList *simplifiedArcs = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	struct PointArrayList *simplifiedArcsEnd;
	simplifiedArcsEnd = simplifiedArcs;

	struct PointArrayList *arcIterator;
	arcIterator = importedStuff->arcs;

	while(arcIterator->next != 0) {
		simplifiedArcsEnd->numPoints = 0;
		if (arcIterator->numPoints < 4) {
			simplifiedArcsEnd->points = arcIterator->points;
			simplifiedArcsEnd->numPoints = arcIterator->numPoints;
		} else {
			// locate each edge;
			struct Edge *locatedEdges[arcIterator->numPoints];
			int i;
			for (i = 0; i < arcIterator->numPoints; i++) {
				struct Point *tmpPoint = &arcIterator->points[i];
				locatedEdges[i] = subdivision_locate(triangulation, tmpPoint);
			}
			// do the stacking/popping of triangles to getFirst a sequence
			// of triangles that the shortest path must visit on its way
			// from start to end
			struct Edge *edgeStack[arcIterator->numPoints+1];
			int edgeNumberStack[arcIterator->numPoints+1];
			int sp;
			sp = 0;
			// push the first edge crossed
			edgeStack[sp++] = locatedEdges[0];
			// for each subsequent edge
			for (i = 1; i < arcIterator->numPoints; i++) {
				if (edge_sym(edgeStack[sp - 1]) == locatedEdges[i]) {
					// if this edge's reverse is on top of the stac, pop it
					sp--;
				} else if (edgeStack[sp - 1] == locatedEdges[i]) {
					// if this edge is on top of the stack then do nothing
				} else {
					// else we're crossing a new edge, push it
					edgeNumberStack[sp] = i;
					edgeStack[sp++] = locatedEdges[i];
				}
			}

			// eliminate any looping around the start point
			// leave the first point, remove up to index sp - 2
			int start;
			start = 1;
			for (i = 2; i < sp - 1; i++) {
				if (edgeIsPartOfRing(edgeStack[i], edgeStack[0])) {
					start = i;
				} else {
					break;
				}
			}
			// eliminate any looping around the end point
			// leave the last point remove up to index 1
			int term;
			term = sp - 2;
			for (i = term - 1; i > 0; i--) {
				if (edgeIsPartOfRing(edgeStack[i], edgeStack[sp - 1])) {
					term = i;
				} else {
					break;
				}
			}
			if (term < start) {
				term = start;
			}
			if (sp < 1) {
				struct Point simplified[2];
				simplified[0] = arcIterator->points[0];
				simplified[1] = arcIterator->points[arcIterator->numPoints - 1];
				simplifiedArcsEnd->points = simplified;
				simplifiedArcsEnd->numPoints = 2;
			} else {
				int size;
				size = term - start + 3;
				int index;
				index = 0;
				struct Point simplified[size];
				simplified[index++] = arcIterator->points[0];
				for (i = start; i <= term; i++) {
					simplified[index++] = arcIterator->points[edgeNumberStack[i]];
				}
				simplified[index] = arcIterator->points[arcIterator->numPoints - 1];
				simplifiedArcsEnd->points = simplified;
				simplifiedArcsEnd->numPoints = size;
			}

		}
		simplifiedArcsEnd->next = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
		simplifiedArcsEnd = simplifiedArcsEnd->next;
		arcIterator = arcIterator->next;
	}
	// restore offset
	arcIterator = simplifiedArcs;
	while(arcIterator->next != 0) {
		int n;
		for (n = 0; n < arcIterator->numPoints; n++) {
			arcIterator->points[n].x += importedStuff->offsetLatitude;
			arcIterator->points[n].y += importedStuff->offsetLongitude;
			// tmp debugging code to print all points
			printf("%lf, %lf\n", arcIterator->points[n].x, arcIterator->points[n].y);
		}
		arcIterator = arcIterator->next;
	}
	exportGML_exportGML(simplifiedArcs, DIR);
}

int edgeIsPartOfRing(struct Edge *test, struct Edge *fromOrigin) {
	struct Edge *e;
	e = fromOrigin;
	do {
		if (e == test || edge_lNext(e) == test || edge_lNext(edge_lNext(e)) == test) {
			return 1;
		}
		e = edge_oNext(e);
	} while (e != fromOrigin);
	return 0;
}