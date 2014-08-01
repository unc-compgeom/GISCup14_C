#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Delaunay.h"
#include "Subdivision.h"
#include "ArcsPointsAndOffsets.h"
#include "PointArrayList.h"
#include "ImportGML.h"
#include "ExportGML.h"
#include "UniquePointList.h"
#include "PointList.h"
#include "Point.h"
#include "Edge.h"

int main(int argc, char *argv[]) {

	// Call the program on the command line like this:
	// Simplify < PointToRemove > < LineInputFilePath > < PointInputFilePath > < OutputFilePath >
	int pointsToRemoveCount;
	sscanf(argv[1], "%d", &pointsToRemoveCount);

	printf(" - Reading data...");
	// IMPORT COORDINATES
	struct ArcsPointsAndOffsets *importedStuff;
	importedStuff = importGML_importGML(argv[2], argv[3]);
	
	int ntripts;

	// one more time, print out the points we just read in
	struct PointArrayList *tmpIt;
	tmpIt = importedStuff->points;
	int tmpI;
	double bbox[4] = {INFINITY, INFINITY, -INFINITY, -INFINITY};
	while (tmpIt) {
		ntripts += 1;
		for (tmpI = 0; tmpI < tmpIt->numPoints; tmpI++) {
			if (bbox[0]>tmpIt->points[tmpI].x) bbox[0] = tmpIt->points[tmpI].x;
			if (bbox[1]>tmpIt->points[tmpI].y) bbox[1] = tmpIt->points[tmpI].y;
			if (bbox[2]<tmpIt->points[tmpI].x) bbox[2] = tmpIt->points[tmpI].x;
			if (bbox[3]<tmpIt->points[tmpI].y) bbox[3] = tmpIt->points[tmpI].y;
		}
		tmpIt = tmpIt->next;
	}
	tmpIt = importedStuff->arcs;
	while (tmpIt) {
		ntripts += 2;
		for (tmpI = 0; tmpI < tmpIt->numPoints; tmpI++) {
			if (bbox[0]>tmpIt->points[tmpI].x) bbox[0] = tmpIt->points[tmpI].x;
			if (bbox[1]>tmpIt->points[tmpI].y) bbox[1] = tmpIt->points[tmpI].y;
			if (bbox[2]<tmpIt->points[tmpI].x) bbox[2] = tmpIt->points[tmpI].x;
			if (bbox[3]<tmpIt->points[tmpI].y) bbox[3] = tmpIt->points[tmpI].y;
		}
		tmpIt = tmpIt->next;
	}
	printf("done\n");


	printf(" - Triangulating points...");
	// Maintain a list of points to triangulate
	struct UniquePointList triPts;
	triPts.pts  = (struct Point*) malloc(ntripts*sizeof(struct Point));
	triPts.ids = (int*) malloc(ntripts*sizeof(int));
	triPts.edges = malloc(ntripts*sizeof(struct Edge));


	// add all constraint points and unique arc endpoints to the triangulation list
	struct PointArrayList *arrayListIterator;
	int i = 0;
	// copy unique front and end points from arcs list
	arrayListIterator = importedStuff->arcs;
	while (arrayListIterator) {
		triPts.pts[i] = arrayListIterator->points[0];
		triPts.pts[i].id = i; // front points are even : 2*arc#
		i++;
		triPts.pts[i] = arrayListIterator->points[arrayListIterator->numPoints-1];
		triPts.pts[i].id = i; // end points are odd: 2*arc#+1
		i++;
		arrayListIterator = arrayListIterator->next;
	}

	int ptIDs = i;  // save start of point ids in case we need to distinguish later.

	// copy all points
	arrayListIterator = importedStuff->points;
	while (arrayListIterator) {
		triPts.pts[i] = arrayListIterator->points[0];
		triPts.pts[i].id = i; // points don't need ids, but we'll assign them for consistency
		i++;
		arrayListIterator = arrayListIterator->next;
	}
	qsort((void *)triPts.pts, ntripts, sizeof(struct Point), point_compar); // print out the points to tri

	// set triPts.ids to first occurrence of coordinates
	int lastpt;
	lastpt = 0;
	triPts.ids[triPts.pts[0].id] = lastpt; // save index of where the point coordinates are in sorted list
	for (i = 1; i<ntripts; i++) {
		printf("%d\n",i);
		if (point_compare(&triPts.pts[i-1], &triPts.pts[i])!=0) // not a repeat
			lastpt = i;
		triPts.ids[triPts.pts[i].id] = lastpt; // save index of where the point coordinates are in sorted list
	}
	// Make a list of the unique points to triangulate, in original order
	struct PointList *triPoints = NULL;
	struct PointList *triIterator = NULL;
	int triPointsSize=0;
	for (i = 0; i<ntripts; i++) {
		if (triPts.pts[triPts.ids[i]].id == i) { // if the point with my id comes from me
			if (triPoints == NULL) {
				triPoints = (struct PointList*) malloc(sizeof(struct PointList));
				triIterator = triPoints;
			} else {
                triIterator->next = (struct PointList*) malloc(sizeof(struct PointList));
			    triIterator = triIterator->next;
			}
			triIterator->point = triPts.pts[triPts.ids[i]];
			triPointsSize++;
		}
	}
	// set the next pointer to null
	triIterator->next = 0;

	// TRIANGULATE
	struct Subdivision *triangulation;
	triangulation = delaunay_triangulate(triPoints, triPointsSize);
	// for each edge e
	// triPts.edges[edge_orig(e).id] = e;
	printf("done\n");
	// SIMPLIFY

	printf(" - Simplifying arcs...");
	// make a struct to hold the simplified data
	struct PointArrayList *simplifiedArcs = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	// an iterator
	struct PointArrayList *simpArcIter;
	// initialize to front of list
	simpArcIter = simplifiedArcs;
	// iterator for raw data
	arrayListIterator = importedStuff->arcs;

	int removedPoints;
	removedPoints = 0;
	while (1) {
		simpArcIter->numPoints = 0;
		if (arrayListIterator->numPoints < 4) {
			// don't worry about short arcs
			simpArcIter->points = arrayListIterator->points;
			simpArcIter->numPoints = arrayListIterator->numPoints;
		} else {
			// locate each edge;
			struct Edge *locatedEdges[arrayListIterator->numPoints];
			int i;
			for (i = 0; i < arrayListIterator->numPoints; i++) {
				locatedEdges[i] = subdivision_locate(triangulation, &arrayListIterator->points[i]);
			}
			// do the stacking/popping of triangles to get a sequence
			// of triangles that the shortest path must visit on its way
			// from start to end
			struct Edge *edgeStack[arrayListIterator->numPoints + 1];
			int edgeNumberStack[arrayListIterator->numPoints + 1];
			int sp;
			sp = 0;
			i = 0;
			// push the first edge crossed
			edgeStack[sp] = locatedEdges[i];
			edgeNumberStack[sp++] = i;
			// for each subsequent edge
			for (i = 1; i < arrayListIterator->numPoints; i++) {
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
			// this code is wrong
			for (i = 2; i < sp - 1; i++) {
				if (predicate_edgeIsPartOfRing(edgeStack[i], edgeStack[0])) {
					start = i;
				} else {
					break;
				}
			}
			// eliminate any looping around the end point
			// leave the last point remove up to index 1
			int term;
			term = sp - 2;
			// this code is wrong
			for (i = term - 1; i > 0; i--) {
				if (predicate_edgeIsPartOfRing(edgeStack[i], edgeStack[sp - 1])) {
					term = i;
				} else {
					break;
				}
			}
			if (term < start) {
				term = start;
			}
			if (sp < 1) {
				struct Point *simplified = (struct Point *) malloc(sizeof(struct Point)*2);
				simplified[0] = arrayListIterator->points[0];
				simplified[1] = arrayListIterator->points[arrayListIterator->numPoints - 1];
				simpArcIter->points = simplified;
				simpArcIter->numPoints = 2;
			} else {
				int size;
				size = term - start + 3;
				int index;
				index = 0;
				struct Point *simplified = (struct Point *) malloc(sizeof(struct Point)*size);
				simplified[index++] = arrayListIterator->points[0];
				for (i = start; i <= term; i++) {
					simplified[index++] = arrayListIterator->points[edgeNumberStack[i]];
				}
				simplified[index] = arrayListIterator->points[arrayListIterator->numPoints - 1];
				simpArcIter->points = simplified;
				simpArcIter->numPoints = size;
				removedPoints += arrayListIterator->numPoints - size;
			}

		}
		if (arrayListIterator->next) {
			simpArcIter->next = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
			simpArcIter = simpArcIter->next;
			arrayListIterator = arrayListIterator->next;
		} else {
			// stop when there are no more point arrays to process
			break;
		}
	}

	// did we stop early?
	// if (arrayListIterator->next) {
	// 	while(1) {
	// 		simpArcIter->points = arrayListIterator->points;
	// 		simpArcIter->numPoints = arrayListIterator->numPoints;
	// 		if (arrayListIterator->next) {
	// 			simpArcIter->next = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	// 			simpArcIter = simpArcIter->next;
	// 			arrayListIterator = arrayListIterator->next;
	// 		} else {
	// 			break;
	// 		}
	// 	}
	// }

	printf("done\n");
	// restore offset
	arrayListIterator = simplifiedArcs;
	while(arrayListIterator) {
		int n;
		for (n = 0; n < arrayListIterator->numPoints; n++) {
			arrayListIterator->points[n].x += importedStuff->offsetLongitude;
			arrayListIterator->points[n].y += importedStuff->offsetLatitude;
			// tmp debugging code to print all points
		}
		arrayListIterator = arrayListIterator->next;
	}
	exportGML_exportGML(simplifiedArcs, argv[4]);
	printf("Simplified by %f percent\n", (removedPoints*100.0) / (double) importedStuff->arcPointCount);
	
	exit(0);
}
