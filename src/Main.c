#include <stdio.h>
#include <stdlib.h>
#include "Delaunay.h"
#include "Subdivision.h"
#include "ArcsPointsAndOffsets.h"
#include "ImportGML.h"
#include "PointList.h"

int main() {
	struct ArcsPointsAndOffsets *importedStuff;
	importedStuff = importGML_importGML("../td1/lines_out.txt", "../td1/points_out.txt");
	
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
			printf("%lf, %lf\n", listIterator->points[i].x, listIterator->points[i].y);
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
		printf("%lf, %lf", front->x, front->y);
		struct Point *end;
		end = &listIterator->points[listIterator->numPoints-1];
		printf("%lf, %lf", end->x, end->y);
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

	int n;
	struct PointList *iter;
	iter = triangulationPoints;
	for(n = 0; n < triangulationPointsCount; n++) {
		printf("%lf, %lf\n",iter->point.x, iter->point.y);
		iter = iter->next;
	}

	struct Subdivision *triangulation;
	triangulation = delaunay_triangulate(triangulationPoints, triangulationPointsCount);
}