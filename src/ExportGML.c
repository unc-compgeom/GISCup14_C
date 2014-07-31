#include "ExportGML.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILENAME simplified.txt

void exportGML_exportGML(struct PointArrayList *PointArrayList, char directoryName[]) {
	FILE * fp;

	int newFileNameLength;
	newFileNameLength = strlen(directoryName) + strlen(FILENAME);
	char newFileName[newFileNameLength];
	strcat(newFileName, directoryName);
	strcat(newFileName, FILENAME);
	fp = fopen(newFileName, "w");
	if (!fp) {
		exit(1);
	}

}