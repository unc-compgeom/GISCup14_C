#include "ImportGML.h"
#include "ArcsPointsAndOffsets.h"
#include <stdio.h>
#include <stdlib.h>

struct PointsList * readFile(char fileName[]) {
	File *file;
	file = fopen(fileName, "r");
	if (!file) {
		printf("Error opening file %s\n", fileName);
		exit(1);
	}
	char c;
	double coordinate;
	double x, y;
	while(1) {
		while(1) {
			fscanf(file, "%c", c);
			if (c == '>') {
				break;
			}
		}
		while(1) {
			fscanf(file, "%c", c);
			if (c == '>') {
				break;
			}
		}
		fscanf(file, "%lf,%lf ", &x, &y);
		struct CoordinateList *coordinateList = {x, y, 0};
		while(1) {
			if(fscanf(file, "%lf,%lf ", &x, &y) == 0) {
				break;
			}
			struct CoordinateList *newCoords = {x, y, 0};
			coordinateList->next = newCoords;
			coordinateList = newCoords;
		}

		struct PointsList *p = (struct PointsList*) malloc(sizeof(struct PointsList));
		return p;
	}
}

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]) {
	struct ArcsPointsAndOffsets *data;
	data = arcsPointsAndOffsets_construct();
	struct PointsList *readArcs;
	readArcs = readFile(arcsFilename);
	struct PointsList *readPoints;
	readPoints = readFile(pointsFilename);

	return data;
}