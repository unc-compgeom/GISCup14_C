#ifndef CG_DELAUNAY_H
#define CG_DELAUNAY_H

#include "Point.h"
#include "PointList.h"

struct Subdivision * delaunay_triangulate(struct PointList *points, int numPoints, struct Point *bboxLL, struct Point *bboxUR);

#endif /* CG_DELAUNAY_H */