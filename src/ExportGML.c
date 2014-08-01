#include "ExportGML.h"
#include "Point.h"
#include "PointArrayList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILENAME "/simplified.txt"

void exportGML_exportGML(struct PointArrayList *pointArrayList, char directoryName[]) {
	FILE * fp;

	char *newFileName;
	strcat(newFileName, directoryName);
	strcat(newFileName, FILENAME);
	fp = fopen(newFileName, "w");
	if (!fp) {
		exit(1);
	}
	int i;
	int j;
	i = 1;
	
	struct PointArrayList *listIterator;
	listIterator = pointArrayList;
	while (listIterator) {
		fprintf(fp, "%d:<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">", i++);
		for (j = 0; j < listIterator->numPoints; j++) {
			fprintf(fp, "%lf,%lf ", listIterator->points[j].x, listIterator->points[j].y);
			printf("%lf,%lf\n ", listIterator->points[j].x, listIterator->points[j].y);
		}
		fprintf(fp, "</gml:coordinates></gml:LineString>\n");
		listIterator = listIterator->next;
	}
	fclose(fp);
}