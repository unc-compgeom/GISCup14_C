#include "Point.h"
#include "QuadEdge.h"
#include "Subdivision.h"
#include "Delaunay.h"
#include <stdlib.h>
#include <stdio.h>

///////////////// DELAUNAY TRIANGULATION /////////////////
struct Subdivision * delaunay_triangulate(struct Point points[], int numPoints) {
	struct Subdivision *s;
	s = subdivision_construct();
	int i;
	printf("  Constructed subdivision\n");
	for (i = 0; i < numPoints; i++) {
		subdivision_insertSite(s, &points[i]);
		printf("  Inserted point %d\n", i);
	}
	return s;
}