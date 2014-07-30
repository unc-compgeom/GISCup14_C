#include "Point.h"
#include "QuadEdge.h"
#include "Subdivision.h"
#include "Delaunay.h"

///////////////// DELAUNAY TRIANGULATION /////////////////
struct Subdivision * delaunay_triangulate(struct Point points[], int numPoints) {
	struct Subdivision *s;
	struct QuadEdge *qe;
	subdivision_construct(s, qe);
	int i;
	for (i = 0; i < numPoints; i++) {
		subdivision_insertSite(s, &points[i]);
	}
	return s;
}