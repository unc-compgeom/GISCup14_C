#ifndef CG_IMPORTGML_H
#define CG_IMPORTGML_H

#include "ArcsPointsAndOffsets.h"
#include "PointArrayList.h"

struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct();

struct PointArrayList * importGML_readFile(char fileName[]);

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]);

#endif /* #ifndef CG_IMPORTGML_H */