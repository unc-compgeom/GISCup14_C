#ifndef CG_UNIQUEPOINTLIST_H
#define CG_UNIQUEPOINTLIST_H

#include "Point.h"

struct UniquePointList {
	int *ids; // index in sorted point list of original point
	struct Point *pts; // array of points to be sorted
	struct Edge *edges;
};

int ptCompare(struct Point * a, struct Point * b);


#endif /* CG_UNIQUEPOINTLIST_H */