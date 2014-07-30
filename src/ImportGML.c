#include "ImportGML.h"
#include "ArcsPointsAndOffsets.h"
#include <stdio.h>

struct CoordinateList {
	double x;
	double y;
	struct CoordinateList *next;
};

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
		fscanf(file, "%d,%d ", &x, &y);
		struct CoordinateList *coordinateList = {x, y, 0};
		while(1) {
			if(fscanf(file, "%d,%d ", &x, &y) == 0) {
				break;
			}
			struct CoordinateList *newCoords = {x, y, 0};
			coordinateList->next = newCoords;
			coordinateList = newCoords;
		}


	}
}

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]) {
	struct ArcsPointsAndOffsets *data = arcsPointsAndOffsets_construct();
	struct PointsList *readArcs;
	readArcs = readFile(arcsFilename);
	struct PointsList *readPoints;
	readPoints = readFile(pointsFilename);

	return data;
}