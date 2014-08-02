#ifndef CG_IMPORTGML_H
#define CG_IMPORTGML_H

#include "ArcsPointsAndOffsets.h"
#include "PointArrayList.h"

void importGML_readFile(struct PointArrayList *destination, char fileName[]);

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]);

#endif /* #ifndef CG_IMPORTGML_H */