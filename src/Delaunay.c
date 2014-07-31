#include "Point.h"
#include "QuadEdge.h"
#include "Subdivision.h"
#include "Delaunay.h"
#include <stdlib.h>
#include <stdio.h>

///////////////// DELAUNAY TRIANGULATION /////////////////
struct Subdivision * delaunay_triangulate(struct PointList *points, int numPoints) {
	struct Subdivision *s;
	s = subdivision_construct();
	int i;
	printf("  Constructed subdivision\n");
	struct PointList *listIterator;
	listIterator = points;
	for (i = 0; i < numPoints; i++) {
		subdivision_insertSite(s, &listIterator->point);
		listIterator = listIterator->next;
		printf("  Inserted point %d\n", i);
	}
	return s;
}