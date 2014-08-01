#include "ImportGML.h"
#include "Point.h"
#include "PointArrayList.h"
#include "ArcsPointsAndOffsets.h"
#include "PointList.h"
#include <stdio.h>
#include <stdlib.h>

 void importGML_readFile(struct PointArrayList *destination, char fileName[]) {
	FILE * fp;
	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Error opening file %s\n", fileName);
		exit(1);
	}

	struct PointArrayList *destinationEnd;
	destinationEnd = destination;
	while(1) {
		char c;
		double x, y;
		int numberOfCoordinates;
		numberOfCoordinates = 0;
		// skip line header information
		fscanf(fp, "%c", &c);
		while(1) {
			fscanf(fp, "%c", &c);
			if (c == '>') {
				break;
			}
		}
		while(1) {
			fscanf(fp, "%c", &c);
			if (c == '>') {
				break;
			}
		}
		fscanf(fp, "%lf,%lf ", &x, &y);
		struct PointList *pointsList = (struct PointList*) malloc(sizeof(struct PointList));
		struct PointList *frontOfList;
		frontOfList = pointsList;
		pointsList->point.x = x;
		pointsList->point.y = y;
		numberOfCoordinates++;
		while(1) {
			if(fscanf(fp, "%lf,%lf ", &x, &y) == 0) {
				break;
			}
			struct PointList *newCoords = (struct PointList*) malloc(sizeof(struct PointList));
			newCoords->point.x = x;
			newCoords->point.y = y;
			pointsList->next = newCoords;
			pointsList = newCoords;
			numberOfCoordinates++;
		}
		// process list into an array
		struct Point *points = (struct Point*) malloc(sizeof(struct Point)*numberOfCoordinates);
		destinationEnd->points = points;
		destinationEnd->numPoints = numberOfCoordinates;
		int i;
		struct PointList *arrayListIterator;
		arrayListIterator = frontOfList;
		for (i = 0; i < numberOfCoordinates; i++) {
			struct Point *tmpPoint = (struct Point*) malloc(sizeof(struct Point));
			tmpPoint->x = arrayListIterator->point.x;
			tmpPoint->y = arrayListIterator->point.y;
			destinationEnd->points[i] = *tmpPoint;
			free(tmpPoint);
			arrayListIterator = arrayListIterator->next;
		}
		destinationEnd->numPoints = numberOfCoordinates;
		destinationEnd->points = points;

		// finish line
		while(1) {
			fscanf(fp, "%c", &c);
			if (c == '>') {
				break;
			}
		}
		while(1) {
			fscanf(fp, "%c", &c);
			if (c == '>') {
				break;
			}
		}
		// go on to the next line
		fscanf(fp, "%c", &c);
		// read the first character of the next line
		fscanf(fp, "%c", &c);
		if (c == '\n') {
			destinationEnd->next = 0;
			break;
		}
		destinationEnd->next = 	(struct PointArrayList*) malloc(sizeof(struct PointArrayList));	
		destinationEnd = destinationEnd->next;
	}
	fclose(fp);
}

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]) {
	struct ArcsPointsAndOffsets *data;
	data = arcsPointsAndOffsets_construct();
	data->arcs 		= (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	data->arcPointCount = 0;
	data->points 	= (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	importGML_readFile(data->arcs, arcsFilename);
	importGML_readFile(data->points, pointsFilename);

	// find the minimum latitude and longitude also count the number of points in the arcs
	double minimumLongitude = data->points->points[0].x;
	double minimumLatitude = data->points->points[0].y;
	int i;
	struct PointArrayList *arrayListIterator;
	
	arrayListIterator = data->arcs;
	while(arrayListIterator) {
		data->arcPointCount += arrayListIterator->numPoints;
		for (i = 0; i < arrayListIterator->numPoints; i++) {
			if (arrayListIterator->points[i].x < minimumLongitude) {
				minimumLongitude = arrayListIterator->points[i].x;
			}
			if (arrayListIterator->points[i].y < minimumLatitude) {
				minimumLatitude = arrayListIterator->points[i].y;
			}
		}
		arrayListIterator = arrayListIterator->next;
	}

	arrayListIterator = data->points;
	while (arrayListIterator) {
		for (i = 0; i < arrayListIterator->numPoints; i++) {
			if (arrayListIterator->points[i].x < minimumLongitude) {
				minimumLongitude = arrayListIterator->points[i].x;
			}
			if (arrayListIterator->points[i].y < minimumLatitude) {
				minimumLatitude = arrayListIterator->points[i].y;
			}
		}
		arrayListIterator = arrayListIterator->next;
	}
	data->offsetLongitude = (long) minimumLongitude;
	data->offsetLatitude= (long) minimumLatitude;
	// subtract offset from points
	arrayListIterator = data->arcs;
	while (arrayListIterator) {
		for (i = 0; i < arrayListIterator->numPoints; i++) {
			arrayListIterator->points[i].x -= data->offsetLongitude;
			arrayListIterator->points[i].y -= data->offsetLatitude;
		}
		arrayListIterator = arrayListIterator->next;
	}
	arrayListIterator = data->points;
	while (arrayListIterator) {
		for (i = 0; i < arrayListIterator->numPoints; i++) {
			arrayListIterator->points[i].x -= data->offsetLongitude;
			arrayListIterator->points[i].y -= data->offsetLatitude;
		}
		arrayListIterator = arrayListIterator->next;
	}

	// done!
	return data;
}
struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct() {
	struct ArcsPointsAndOffsets *data = (struct ArcsPointsAndOffsets*) malloc(sizeof(struct ArcsPointsAndOffsets));
	return data;
}