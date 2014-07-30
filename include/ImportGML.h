#ifndef CG_IMPORTGML_H
#define CG_IMPORTGML_H

#include "ArcsPointsAndOffsets.h"

struct PointsList {
	double x;
	double y;
	struct PointsList *next;
};

struct PointsArrayList {
	int numPoints;
	struct Point points[];
	struct PointsListList *next;
}

struct PointsList * importGML_readFile(char fileName[]);

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]);

#endif /* #ifndef CG_IMPORTGML_H */