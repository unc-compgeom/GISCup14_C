#ifndef CG_ARCSPOINTSANDOFFSETS_H
#define CG_ARCSPOINTSANDOFFSETS_H

#include "PointArrayList.h"

struct ArcsPointsAndOffsets {
	struct PointArrayList *arcs;
	int arcPointCount;
	struct PointArrayList *points;
	long offsetLongitude;
	long offsetLatitude;
};

struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct();

#endif /* CG_ARCSPOINTSANDOFFSETS_H */