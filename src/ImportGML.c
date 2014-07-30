#include "ImportGML.h"
#include "ArcsPointsAndOffsets.h"
#include "Point.h"
#include <stdio.h>
#include <stdlib.h>

struct PointsList * importGML_readFile(char fileName[]) {
	FILE * fp;
	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Error opening file %s\n", fileName);
		exit(1);
	}
	
	struct PointsArrayList *importedPoints = (struct PointsArrayList*) malloc(sizeof(struct PointsArrayList));
	while(1) {
		char c;
		double x, y;
		int numberOfCoordinates;
		numberOfCoordinates = 0;
		// skip line header information
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
		struct PointsList *pointsList = (struct PointsList*) malloc(sizeof(struct PointsList));
		struct PointsList *frontOfList;
		frontOfList = pointsList;
		pointsList->x = x;
		pointsList->y = y;
		numberOfCoordinates++;
		while(1) {
			if(fscanf(fp, "%lf,%lf ", &x, &y) == 0) {
				break;
			}
			struct PointsList *newCoords = (struct PointsList*) malloc(sizeof(struct PointsList));
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
		struct PointsList *listIterator;
		listIterator = frontOfList;
		for (i = 0; i < numberOfCoordinates; i++) {
			struct Point *tmpPoint = (struct Point*) malloc(sizeof(struct Point));
			tmpPoint->x = *(listIterator->x);
			tmpPoint->y = *(listIterator->y);
			importedPoints->points[i] = tmpPoint;
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
		fscanf(fp, "%c", &c);
		if (c == EOF) {
			break;
		}
		importedPoints->next = 	(struct PointsArrayList*) malloc(sizeof(struct PointsArrayList));	
	}
	return importedPoints;
}

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]) {
	struct ArcsPointsAndOffsets *data;
	data = arcsPointsAndOffsets_construct();
	struct PointsList *readArcs;
	readArcs = importGML_readFile(arcsFilename);
	struct PointsList *readPoints;
	readPoints = importGML_readFile(pointsFilename);

	// find the minimum latitude and longitude
	double minimumLatitude = 0;
	return data;
}