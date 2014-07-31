#include <stdio.h>
#include <stdlib.h>
#include "Delaunay.h"
#include "Subdivision.h"
#include "ArcsPointsAndOffsets.h"
#include "ImportGML.h"
#include "PointList.h"
#include "Edge.h"

#define ARCSFILE "../td3/lines_out.txt"
#define POINTSFILE "../td3/points_out.txt"

int main() {
	// IMPORT COORDINATES
	struct ArcsPointsAndOffsets *importedStuff;
	importedStuff = importGML_importGML(ARCSFILE, POINTSFILE);
	
	int triangulationPointsCount;
	triangulationPointsCount = 0;

	struct PointList *triangulationPoints = (struct PointList*) malloc(sizeof(struct PointList));
	struct PointList *triangulationPointsEnd = triangulationPoints;
	
	// add all constraint points and unique arc endpoints to the triangulation list
	int i;
	struct PointArrayList *listIterator;
	listIterator = importedStuff->points;
	while(listIterator->next != 0) {
		for (i = 0; i < listIterator->numPoints; i++) {
			triangulationPointsEnd->point = listIterator->points[i];
			triangulationPointsEnd->next = (struct PointList*) malloc(sizeof(struct PointList));
			triangulationPointsEnd = triangulationPointsEnd->next;
			triangulationPointsCount++;
		}
		listIterator = listIterator->next;
	}
	listIterator = importedStuff->arcs;
	while(listIterator->next != 0) {
		struct Point *front;
		front = &listIterator->points[0];
		struct Point *end;
		end = &listIterator->points[listIterator->numPoints-1];
		// test if these points are already in the list
		struct PointList *secondListIterator = triangulationPoints;
		int shouldInsertFront;
		int shouldInsertEnd;
		shouldInsertFront = 1;
		shouldInsertEnd = 1;
		while (secondListIterator != 0) {
			if (shouldInsertFront && secondListIterator->point.x == front->x && secondListIterator->point.y == front->y) {
				// front is a duplicate
				shouldInsertFront = 0;
			}
			if (shouldInsertEnd && secondListIterator->point.x == end->x && secondListIterator->point.y == end->y) {
				shouldInsertEnd = 0;
			}
			if (!shouldInsertFront && !shouldInsertEnd) {
				break;
			}
			secondListIterator = secondListIterator->next;
		}
		if (shouldInsertFront) {
			triangulationPointsEnd->point = *front;
			triangulationPointsEnd->next = (struct PointList*) malloc(sizeof(struct PointList));
			triangulationPointsEnd = triangulationPointsEnd->next;
			triangulationPointsCount++;
		}
		if (shouldInsertEnd) {
			triangulationPointsEnd->point = *end;
			triangulationPointsEnd->next = (struct PointList*) malloc(sizeof(struct PointList));
			triangulationPointsEnd = triangulationPointsEnd->next;
			triangulationPointsCount++;
		}
		listIterator = listIterator->next;
	}

	// TRIANGULATE
	struct Subdivision *triangulation;
	triangulation = delaunay_triangulate(triangulationPoints, triangulationPointsCount);

	// SIMPLIFY

	struct PointArrayList *simplifiedArcs = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	struct PointArrayList *simplifiedArcsEnd;
	simplifiedArcsEnd = simplifiedArcs;

	struct PointArrayList *arcIterator;
	arcIterator = importedStuff->arcs;

	while(arcIterator->next != 0) {
		if (arcIterator->numPoints < 4) {
			simplifiedArcsEnd->points = arcIterator->points;
		} else {
			// locate each edge;
			struct Edge locatedEdges[arcIterator->numPoints];
			int i;
			for (i = 0; i < arcIterator->numPoints; i++) {
				locatedEdges[i] = subdivision_locate(triangulation, &arcIterator->points[i]);
			}
			// do the stacking/popping of triangles to getFirst a sequence
			// of triangles that the shortest path must visit on its way
			// from start to end
			struct Edge edgeStack[arcIterator->numPoints+1];
			int edgeNumberStack[arcIterator->numPoints+1];
			int sp;
			sp = 0;
			// push the first edge crossed
			edgeStack[sp++] = locatedEdges[0];
			// for each subsequent edge
			for (i = 1; i < arcIterator->numPoints; i++) {
				if (edge_sym(&edgeStack[sp - 1]) == &locatedEdges[i]) {
					// if this edge's reverse is on top of the stac, pop it
					sp--;
				} else if (&edgeStack[sp - 1] == &locatedEdges[i]) {
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
				simplifiedArcs->point = simplified;
			}

		}
		simplifiedArcsEnd->next = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
		simplifiedArcsEnd = simplifiedArcsEnd->next;
		arcIterator = arcIterator->next;
	}
}