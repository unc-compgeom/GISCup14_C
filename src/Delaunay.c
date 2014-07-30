#include "Point.h"
#include "QuadEdge.h"
#include "Subdivision.h"
#include "Delaunay.h"
#include <stdlib.h>
#include <stdio.h>

///////////////// DELAUNAY TRIANGULATION /////////////////
struct Subdivision * delaunay_triangulate(struct Point points[], int numPoints) {
	struct Subdivision *s = (struct Subdivision*) malloc(sizeof(struct Subdivision));
	subdivision_construct(s);
	int i;
	printf("constructed subdivision\n");
	for (i = 0; i < numPoints; i++) {
		subdivision_insertSite(s, &points[i]);
		printf("inserted point %d\n", i);
	}
	return s;
}