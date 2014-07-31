#include "ImportGML.h"
#include "Point.h"
#include "PointArrayList.h"
#include "ArcsPointsAndOffsets.h"
#include "PointList.h"
#include <stdio.h>
#include <stdlib.h>

struct PointArrayList * importGML_readFile(char fileName[]) {
	FILE * fp;
	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Error opening file %s\n", fileName);
		exit(1);
	}

	struct PointArrayList *importedPoints = (struct PointArrayList*) malloc(sizeof(struct PointArrayList));
	
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
		pointsList->x = x;
		pointsList->y = y;
		numberOfCoordinates++;
		while(1) {
			if(fscanf(fp, "%lf,%lf ", &x, &y) == 0) {
				break;
			}
			struct PointList *newCoords = (struct PointList*) malloc(sizeof(struct PointList));
			newCoords->x = x;
			newCoords->y = y;
			pointsList->next = newCoords;
			pointsList = newCoords;
			numberOfCoordinates++;
		}
		// process list into an array
		struct Point *points = (struct Point*) malloc(sizeof(struct Point)*numberOfCoordinates);
		importedPoints->points = points;
		importedPoints->numPoints = numberOfCoordinates;
		int i;
		struct PointList *listIterator;
		listIterator = frontOfList;
		for (i = 0; i < numberOfCoordinates; i++) {
			struct Point *tmpPoint = (struct Point*) malloc(sizeof(struct Point));
			tmpPoint->x = listIterator->x;
			tmpPoint->y = listIterator->y;
			importedPoints->points[i] = *tmpPoint;
			free(tmpPoint);
			listIterator = listIterator->next;
		}
		importedPoints->numPoints = numberOfCoordinates;
		importedPoints->points = points;

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
		importedPoints->next = 	(struct PointArrayList*) malloc(sizeof(struct PointArrayList));	
	}
	return importedPoints;
}

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]) {
	struct ArcsPointsAndOffsets *data;
	data = arcsPointsAndOffsets_construct();
	data->arcs = importGML_readFile(arcsFilename);
	data->points = importGML_readFile(pointsFilename);

	// find the minimum latitude and longitude
	double minimumLatitude = data->arcs->points[0].x;
	double minimumLongitude = data->arcs->points[0].y;
	int i;
	struct PointArrayList *listIterator;
	listIterator = data->arcs;
	while(listIterator->next != 0) {
		for (i = 0; i < listIterator->numPoints; i++) {
			if (listIterator->points[i].x < minimumLatitude) {
				minimumLatitude = listIterator->points[i].x;
			}
			if (listIterator->points[i].y < minimumLongitude) {
				minimumLongitude = listIterator->points[i].y;
			}
		}
		listIterator = listIterator->next;
	}
	listIterator = data->points;
	while(listIterator->next != 0) {
		for (i = 0; i < listIterator->numPoints; i++) {
			if (listIterator->points[i].x < minimumLatitude) {
				minimumLatitude = listIterator->points[i].x;
			}
			if (listIterator->points[i].y < minimumLongitude) {
				minimumLongitude = listIterator->points[i].y;
			}
		}
		listIterator = listIterator->next;
	}
	data->offsetLatitude = (long) minimumLatitude;
	data->offsetLongitude = (long) minimumLongitude;
	listIterator = data->arcs;
	while(listIterator->next != 0) {
		for (i = 0; i < listIterator->numPoints; i++) {
			listIterator->points[i].x -= data->offsetLatitude;
			listIterator->points[i].y -= data->offsetLongitude;
		}
		listIterator = listIterator->next;
	}
	listIterator = data->points;
	while(listIterator->next != 0) {
		for (i = 0; i < listIterator->numPoints; i++) {
			listIterator->points[i].x -= data->offsetLatitude;
			listIterator->points[i].y -= data->offsetLongitude;
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