#ifndef CG_POINTLIST_H
#define CG_POINTLIST_H

#include "Point.h"

struct PointList {
	struct Point point;
	struct PointList *next;
};

#endif /* CG_POINTLIST_H */