#ifndef CG_POINTARRAYLIST
#define CG_POINTARRAYLIST

#include "Point.h"

struct PointArrayList {
	int numPoints;
	struct Point *points;
	struct PointArrayList *next;
};

#endif /* CG_POINTARRAYLIST */