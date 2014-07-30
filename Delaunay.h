#ifndef CG_DELAUNAY_H
#define CG_DELAUNAY_H

#include "Point.h"

struct Subdivision * triangulate(struct Point points[], int numPoints);

#endif /* CG_DELAUNAY_H */