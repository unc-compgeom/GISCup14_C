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
			triangulationPointsEnd->point = listIterator->points[i];
			triangulationPointsEnd->next = (struct PointList*) malloc(sizeof(struct PointList));
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
		}
		if (shouldInsertFront) {
			triangulationPointsEnd->point = *front;
			triangulationPointsEnd->next = (struct PointList*) malloc(sizeof(struct PointList));
			triangulationPointsCount++;
		}
		if (shouldInsertEnd) {
			triangulationPointsEnd->point = *end;
			triangulationPointsEnd->next = (struct PointList*) malloc(sizeof(struct PointList));
			triangulationPointsCount++;
		}
		listIterator = listIterator->next;
	}
	/*
	printf("TEST:\n");
	printf("Generate sample points\n");
	struct Point points[10];
	struct Point tmp1 	= 	{0,0};
	struct Point tmp2 	= 	{1,0};
	struct Point tmp3 	= 	{0,2};
	struct Point tmp4 	= 	{3,1};
	struct Point tmp5 	= 	{3,6};
	struct Point tmp6 	= 	{2,0};
	struct Point tmp7 	= 	{1,9};
	struct Point tmp8 	= 	{1,7};
	struct Point tmp9	= 	{2,4};
	struct Point tmp10 	= 	{80, 10};
	printf("  Made ten points\n");
	int i;
	i = 0;
	points[i++] = tmp1;
	points[i++] = tmp2;
	points[i++] = tmp3;
	points[i++] = tmp4;
	points[i++] = tmp5;
	points[i++] = tmp6;
	points[i++] = tmp7;
	points[i++] = tmp8;
	points[i++] = tmp9;
	points[i++] = tmp10;
	printf("  Inserted ten points\n");
	printf("Began triangulation\n");
	struct Subdivision *triangulation;
	triangulation = delaunay_triangulate(points, 10);
	*/
}