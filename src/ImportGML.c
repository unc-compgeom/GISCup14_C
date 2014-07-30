#include "ImportGML.h"
#include "ArcsPointsAndOffsets.h"
#include <stdio.h>
#include <stdlib.h>

struct PointsList * importGML_readFile(char fileName[]) {
	FILE * fp;
	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Error opening file %s\n", fileName);
		exit(1);
	}
	char c;
	double coordinate;
	double x, y;
	while(1) {
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
		printf("%lf, %lf ", x, y);
		struct CoordinateList *coordinateList = (struct CoordinateList*) malloc(sizeof(struct CoordinateList));
		coordinateList.x = x;
		coordinateList.y = y;
		while(1) {
			if(fscanf(fp, "%lf,%lf ", &x, &y) == 0) {
				break;
			}
			printf("%lf, %lf ", x, y);
			struct CoordinateList *newCoords = (struct CoordinateList*) malloc(sizeof(struct CoordinateList));
			newCoords.x = x;
			newCoords.y = y;
			coordinateList->next = newCoords;
			coordinateList = newCoords;
		}
		printf("done");
		struct PointsList *p = (struct PointsList*) malloc(sizeof(struct PointsList));
		return p;
	}
}

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]) {
	struct ArcsPointsAndOffsets *data;
	data = arcsPointsAndOffsets_construct();
	struct PointsList *readArcs;
	readArcs = importGML_readFile(arcsFilename);
	struct PointsList *readPoints;
	readPoints = importGML_readFile(pointsFilename);

	return data;
}