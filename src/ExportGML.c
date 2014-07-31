#include "ExportGML.h"
#include <stdlib.h>
#include <stdio.h>
#define FILENAME simplified.txt

void exportGML_exportGML(struct PointArrayList *PointArrayList), char[] directoryName) {
	FILE * fp;
	fp = fopen(directoryName FILENAME, "r");
	if (!fp) {
		printf("Error opening file %s\n", directoryName FILENAME);
		exit(1);
	}

}