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
		struct PointList *listIterator;
		listIterator = frontOfList;
		for (i = 0; i < numberOfCoordinates; i++) {
			struct Point *tmpPoint = (struct Point*) malloc(sizeof(struct Point));
			tmpPoint->x = listIterator->point.x;
			tmpPoint->y = listIterator->point.y;
			destinationEnd->points[i] = *tmpPoint;
			free(tmpPoint);
			listIterator = listIterator->next;
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
	data->points 	= (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	importGML_readFile(data->arcs, arcsFilename);
	importGML_readFile(data->points, pointsFilename);

	// find the minimum latitude and longitude
	double minimumLongitude = data->arcs->points[0].x;
	double minimumLatitude = data->arcs->points[0].y;
	int i;
	struct PointArrayList *listIterator;
	listIterator = data->arcs;
	while(listIterator->next != 0) {
		for (i = 0; i < listIterator->numPoints; i++) {
			if (listIterator->points[i].x < minimumLongitude) {
				minimumLongitude = listIterator->points[i].x;
			}
			if (listIterator->points[i].y < minimumLatitude) {
				minimumLatitude = listIterator->points[i].y;
			}
		}
		listIterator = listIterator->next;
	}
	listIterator = data->points;
	while(listIterator->next != 0) {
		for (i = 0; i < listIterator->numPoints; i++) {
			if (listIterator->points[i].x < minimumLongitude) {
				minimumLongitude = listIterator->points[i].x;
			}
			if (listIterator->points[i].y < minimumLatitude) {
				minimumLatitude = listIterator->points[i].y;
			}
		}
		listIterator = listIterator->next;
	}
	data->offsetLongitude = (long) minimumLongitude;
	data->offsetLatitude= (long) minimumLatitude;
	printf("offsetLongitude %ld\n", data->offsetLongitude);
	printf("offsetLatitude %ld\n", data->offsetLatitude);
	// subtract offset from points
	listIterator = data->arcs;
	while (listIterator) {
		for (i = 0; i < listIterator->numPoints; i++) {
			listIterator->points[i].x -= data->offsetLongitude;
			listIterator->points[i].y -= data->offsetLatitude;
		}
		listIterator = listIterator->next;
	}
	listIterator = data->points;
	while (listIterator) {
		for (i = 0; i < listIterator->numPoints; i++) {
			listIterator->points[i].x -= data->offsetLongitude;
			listIterator->points[i].y -= data->offsetLatitude;
		}
		listIterator = listIterator->next;
	}

	// done!
	return data;
}
struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct() {
	struct ArcsPointsAndOffsets *data = (struct ArcsPointsAndOffsets*) malloc(sizeof(struct ArcsPointsAndOffsets));
	return data;
}