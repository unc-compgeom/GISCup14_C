#include "Point.h"
#include "QuadEdge.h"
#include "Subdivision.h"
#include "Delaunay.h"
#include "PointList.h"
#include <stdlib.h>

///////////////// DELAUNAY TRIANGULATION /////////////////
struct Subdivision * delaunay_triangulate(struct PointList *points, int numPoints, struct Point *bboxLL, struct Point *bboxUR) {
	struct Subdivision *s;
	s = subdivision_construct(bboxLL, bboxUR);
	int i;
	struct PointList *listIterator;
	listIterator = points;
	for (i = 0; i < numPoints; i++) {
		subdivision_insertSite(s, &listIterator->point);
		listIterator = listIterator->next;
	}
	return s;
}