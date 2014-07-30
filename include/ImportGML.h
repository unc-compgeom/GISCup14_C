#ifndef CG_IMPORTGML_H
#define CG_IMPORTGML_H

#include "ArcsPointsAndOffsets.h"

struct CoordinateList {
	double x;
	double y;
	struct CoordinateList *next;
};

struct PointsList * importGML_readFile(char fileName[]);

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]);

#endif /* #ifndef CG_IMPORTGML_H */