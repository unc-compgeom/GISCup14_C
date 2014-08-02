#ifndef CG_UNIQUEPOINTLIST_H
#define CG_UNIQUEPOINTLIST_H

#include "Point.h"
#include "Edge.h"

struct UniquePointList {
	int *ids; // index in sorted point list of original point
	struct Point *pts; // array of points to be sorted
	struct Edge **edges;
};

#endif /* CG_UNIQUEPOINTLIST_H */