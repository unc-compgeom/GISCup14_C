#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Delaunay.h"
#include "Subdivision.h"
#include "ArcsPointsAndOffsets.h"
#include "Point.h"
#include "PointArrayList.h"
#include "ImportGML.h"
#include "ExportGML.h"
#include "UniquePointList.h"
#include "Predicate.h"
#include "Edge.h"


int main(int argc, char *argv[]) {
	
	// Call the program on the command line like this:
	// Simplify < PointToRemove > < LineInputFilePath > < PointInputFilePath > < OutputFilePath >
	int pointsToRemoveCount;
	sscanf(argv[1], "%d", &pointsToRemoveCount);
	
	printf("Reading data...");
	// IMPORT COORDINATES
	struct ArcsPointsAndOffsets *importedStuff;
	importedStuff = importGML_importGML(argv[2], argv[3]);
	
	int ntripts; // count points for triangulation
	ntripts = 0;
	// one more time, print out the points we just read in
	struct PointArrayList *tmpIt;
	tmpIt = importedStuff->points;
	int tmpI;
	int tmpI2;
	tmpI2 = 1;
	printf("points\n");
	double bbox[4] = {INFINITY, INFINITY, -INFINITY, -INFINITY};
	while (tmpIt) {
		printf(" row %d\n", tmpI2++);
		ntripts += 1;
		for (tmpI = 0; tmpI < tmpIt->numPoints; tmpI++) {
			if (bbox[0]>tmpIt->points[tmpI].x) bbox[0] = tmpIt->points[tmpI].x;
			if (bbox[1]>tmpIt->points[tmpI].y) bbox[1] = tmpIt->points[tmpI].y;
			if (bbox[2]<tmpIt->points[tmpI].x) bbox[2] = tmpIt->points[tmpI].x;
			if (bbox[3]<tmpIt->points[tmpI].y) bbox[3] = tmpIt->points[tmpI].y;
			printf("  %lf, %lf\n", tmpIt->points[tmpI].x + importedStuff->offsetLongitude, tmpIt->points[tmpI].y + importedStuff->offsetLatitude);
		}
		tmpIt = tmpIt->next;
	}
	printf("arcs\n");
	tmpI2 = 1;
	tmpIt = importedStuff->arcs;
	while (tmpIt) {
		printf(" row %d\n", tmpI2++);
		ntripts += 2;
		for (tmpI = 0; tmpI < tmpIt->numPoints; tmpI++) {
			if (bbox[0]>tmpIt->points[tmpI].x) bbox[0] = tmpIt->points[tmpI].x;
			if (bbox[1]>tmpIt->points[tmpI].y) bbox[1] = tmpIt->points[tmpI].y;
			if (bbox[2]<tmpIt->points[tmpI].x) bbox[2] = tmpIt->points[tmpI].x;
			if (bbox[3]<tmpIt->points[tmpI].y) bbox[3] = tmpIt->points[tmpI].y;
			printf("  %lf, %lf\n", tmpIt->points[tmpI].x + importedStuff->offsetLongitude, tmpIt->points[tmpI].y + importedStuff->offsetLatitude);
		}
		tmpIt = tmpIt->next;
	}
	printf("done\n");
	
	printf("Triangulating points...");
	
	// Maintain a list of points to triangulate
	struct UniquePointList triPts;
	triPts.pts = (struct Point*) malloc(ntripts*sizeof(struct Point));
	triPts.ids = (int*)malloc(ntripts*sizeof(int));
	triPts.edges = malloc(ntripts*sizeof(struct Edge));
	
	struct PointArrayList *arrayListIterator;
	int i = 0; 
	
	// add all constraint points and unique arc endpoints to the triangulation list
	
	// copy front and end points from arcs list
	arrayListIterator = importedStuff->arcs;
	while (arrayListIterator) {
		triPts.pts[i] = arrayListIterator->points[0];
		triPts.pts[i].id = i;  // front points are even: 2*arc#
		i++;
		triPts.pts[i] = arrayListIterator->points[arrayListIterator->numPoints-1];
		triPts.pts[i].id = i; // end points are odd: 2*arc#+1
		i++;
		arrayListIterator = arrayListIterator->next;
	}
	int ptIDs = i; // save start of point ids in case we need to distinguish later. 
	// copy all points
	arrayListIterator = importedStuff->points;
	while (arrayListIterator) {
		triPts.pts[i] = arrayListIterator->points[0];
		triPts.pts[i].id = i; // points don't need ids, but we'll assign them for consistency
		i++;
		arrayListIterator = arrayListIterator->next;
	}
	// FIND UNIQUE POINTS
	qsort((void *)triPts.pts, ntripts, sizeof(struct Point), point_compare);
	
	// print out the points to triangulate
	printf("triangulation points, sorted\n");
	for (i=0; i<ntripts; i++){
		printf("%5d:  %lf, %lf  (%lf, %lf)\n", triPts.pts[i].id, triPts.pts[i].x + importedStuff->offsetLongitude, triPts.pts[i].y + importedStuff->offsetLatitude, 
			   triPts.pts[i].x, triPts.pts[i].y);
	}
	/* set triPts.ids to first occurrence of coordinates.
	 * After this we will have triPts.pts[triPts.ids[i]] being the coordinates that were originally at i. 
	 * Moreover, all points with the same coordinates will have the same value for triPts.ids[i].
	 * Only those points with orig index i == triPts.pts[triPts.ids[i]].id will be in the triangulation. 
	 * Said in the other way, sorted index j is a point in the triangulation iff j == triPts.ids[triPts.pts[j].id]
     * For each sorted point, assign an edge origin: triPts.edges[edge_orig(e).id] = e. 
	 * For each original point i, the edge with that as origin is triPts.edge[triPts.id[i]]
	 */
	int lastpt = 0;
	triPts.ids[triPts.pts[0].id] = lastpt; // save index of where the point coordinates are in sorted list
	for (i = 1; i<ntripts; i++) {
		if (point_compare(&triPts.pts[i-1], &triPts.pts[i])!=0) // not a repeat
			lastpt = i;
		triPts.ids[triPts.pts[i].id] = lastpt; // save index of where the point coordinates are in sorted list
	}
	// Make a list of the unique points to triangulate, in original order
	// Should add bounding box points?
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
	
	// print out the points to triangulate
	struct PointList *tmpTriIt;
	tmpTriIt = triPoints;
	printf("triangulation points\n");
	while (tmpTriIt) {
		printf("%5d:  %lf, %lf  (%lf, %lf)\n", tmpTriIt->point.id, tmpTriIt->point.x + importedStuff->offsetLongitude, tmpTriIt->point.y + importedStuff->offsetLatitude, tmpTriIt->point.x, tmpTriIt->point.y);
		tmpTriIt = tmpTriIt->next;
	}	
	
	// TRIANGULATE
	struct Subdivision *triangulation;
	struct Point bboxLL = {bbox[0], bbox[1], -1};
	struct Point bboxUR = {bbox[2], bbox[3], -1};
	triangulation = delaunay_triangulate(triPoints, triPointsSize, &bboxLL, &bboxUR);
	
	struct Edge *e;
	e = triangulation->startingEdge;
	while (e) { // for each edge e
		if (edge_orig(e)->id>=0)
			triPts.edges[triPts.ids[edge_orig(e)->id]] = e;
		if (edge_dest(e)->id>=0)
			triPts.edges[triPts.ids[edge_dest(e)->id]] = edge_sym(e);
		e = subdivision_nextEdge(triangulation, e);
	}
	
	printf("done\n");
	// SIMPLIFY
	
	printf("Simplifying arcs...");
	// make a struct to hold the simplified data
	struct PointArrayList *simplifiedArcs = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	// an iterator
	struct PointArrayList *simpArcIter;
	// initialize to front of list
	simpArcIter = simplifiedArcs;
	// iterator for raw data
	arrayListIterator = importedStuff->arcs;
	
	struct Edge *edgeStack[6*triPointsSize]; // make stacks big enough to hold all triangle edges.  Allocate once
	int arcNumberStack[6*triPointsSize]; // index in arc of point that crosses edge
	int removedPoints;
	removedPoints = 0;

	int arcno;
	for (arcno = 0; arcno<ptIDs && removedPoints <= pointsToRemoveCount; i++) {// loop over each arc
		simpArcIter->numPoints = 0;
		if (arrayListIterator->numPoints < 4) {
			// ignore short arcs
			simpArcIter->points = arrayListIterator->points;
			simpArcIter->numPoints = arrayListIterator->numPoints;
		} else { 
			/* Here is where we follow an arc through the triangulation, 
			 stacking the edges that get crossed from L to R,
			 but popping whenever we cross back over an edge that we just crossed. 
			 We have an edge the line pq entered on.  Find the exit edge for the line pq.
			 Then decide if q exits or not. 
			 If it does, stack (or pop) the edge.  
			 If not, find the new entry edge. 
			 Points on edges will be perturbed to be just outside the triangle. 
			 Vertices are perturbed to be L of pq. 
			 */
			int start = triPts.ids[2*arcno]; // find which point knows its edge
			struct Edge *e = triPts.edges[start]; // edge with start point as orig
			struct Point *p = &arrayListIterator->points[0]; 
			if (point_compare(p,edge_orig(e))!=0) // should have start point at origin
				printf("should have start point at origin");
			int nq = 1;
			struct Point *q = &arrayListIterator->points[1];
			while (!predicate_rightOrAhead(q, edge_dest(e), edge_orig(e))) {
				e = edge_oPrev(e); // start with edge that had next point to left or behind
			}
			
			int sp = -1;
			while (nq < arrayListIterator->numPoints) { // while arc points remain
				if (predicate_rightOrAhead(edge_dest(edge_oNext(e)), p, q))
					e = edge_oNext(e);
				else 
					e = edge_dPrev(e); 
				/* We have the next edge the line crosses; now see if pq actually does cross it. */
				if (q == edge_dest(e)) 
					break; // we reached the end point
                else {
					if (predicate_leftOrAhead(q, edge_orig(e), edge_dest(e))) {
						// we continue into next triangle 
						if ((sp>=0) && (edgeStack[sp]==edge_sym(e)))
							sp--;
						else {
							edgeStack[++sp] = e;
							arcNumberStack[sp] = nq;
						}
					} else {
						p = q; // we reached the end of the arcedge in triangle to the right of e
						q = &arrayListIterator->points[++nq];
						e = edge_sym(e);
						// Find the triangle edge that the line pq would have entered.
						double d0 = predicate_triArea(edge_orig(e),p,q); // triangle right is d0,d1,d2 ccw
						double d1 = predicate_triArea(edge_dest(e),p,q); 						
						double d2 = predicate_triArea(edge_dest(edge_oNext(e)),p,q);
						//if (d0>0 && d1<=0) || (d0>=0 && d1<0) e = e;
						if ((d1>0 && d2<=0) || (d1>=0 && d2<0)) 
							e = edge_lNext(e);
						else if ((d2>0 && d0<=0) || (d2>=0 && d0<0)) 
							e = edge_lPrev(e);
						
					}  
				}
			}
			
			// eliminate any looping around the start point
			while (sp >= 0 && point_compare(edge_dest(edgeStack[sp]), 
											&arrayListIterator->points[arrayListIterator->numPoints-1]) ==0)
				sp--;
			start = 0;
			while (start <= sp && point_compare(edge_orig(edgeStack[start]), 
												&arrayListIterator->points[0]) ==0)
				start++;
			
			int count = 1;
			p = &arrayListIterator->points[0];
			for (i=start; i<=sp; i++) {
				q = &arrayListIterator->points[arcNumberStack[i]-1];
				if (point_compare(p, q)!=0) {
					p = q;
					count++;
				}
				q = &arrayListIterator->points[arcNumberStack[i]];
				if (point_compare(p, q)!=0) {
					p = q;
					count++;
				}
			}
			q = &arrayListIterator->points[arrayListIterator->numPoints-1];
			if (point_compare(p, q)!=0) {
				p = q;
				count++;
			}
			
			struct Point *simplified = (struct Point *) malloc(sizeof(struct Point)*count);
			simpArcIter->numPoints = count;
			p = &arrayListIterator->points[0];
			simplified[0] = *p;
			count = 1;
			for (i=start; i<=sp; i++) {
				q = &arrayListIterator->points[arcNumberStack[i]-1];
				if (point_compare(p, q)!=0) {
					p = q;
					simplified[count++] = *p;
				}
				q = &arrayListIterator->points[arcNumberStack[i]];
				if (point_compare(p, q)!=0) {
					p = q;
					simplified[count++] = *p;
				}
			}
			q = &arrayListIterator->points[arrayListIterator->numPoints-1];
			if (point_compare(p, q)!=0) {
				p = q;
				simplified[count++] = *p;
			}
			simpArcIter->points = simplified;
			removedPoints += arrayListIterator->numPoints - count;
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
	for (arcno = arcno; arcno<ptIDs && removedPoints <= pointsToRemoveCount; i++) {// loop over each arc
		simpArcIter->points = arrayListIterator->points;
		simpArcIter->numPoints = arrayListIterator->numPoints;
		if (arrayListIterator->next) {
			simpArcIter->next = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
			simpArcIter = simpArcIter->next;
			arrayListIterator = arrayListIterator->next;
		} else {
			// stop when there are no more point arrays to process
			break;
		}
	}
	
	printf("done\n");
	// restore offset
	arrayListIterator = simplifiedArcs;
	int simpCount;
	simpCount = 1;
	printf("Simplified arcs\n");
	while(arrayListIterator) {
		//printf(" line %d\n", simpCount++);
		printf("],\n[");
		int n;
		for (n = 0; n < arrayListIterator->numPoints; n++) {
			arrayListIterator->points[n].x += importedStuff->offsetLongitude;
			arrayListIterator->points[n].y += importedStuff->offsetLatitude;
			// tmp debugging code to print all points
			printf("  %lf, %lf\n", arrayListIterator->points[n].x, arrayListIterator->points[n].y);
		}
		arrayListIterator = arrayListIterator->next;
	}
	exportGML_exportGML(simplifiedArcs, argv[4]);
	printf("removed %d points\n", removedPoints);
}

