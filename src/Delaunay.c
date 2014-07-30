#include "Point.h"
#include "QuadEdge.h"
#include "Subdivision.h"
#include "Delaunay.h"
#include <stdlib.h>
#include <stdio.h>

///////////////// DELAUNAY TRIANGULATION /////////////////
struct Subdivision * delaunay_triangulate(struct Point points[], int numPoints) {
	struct Subdivision *s = (struct Subdivision*) malloc(sizeof(struct Subdivision));
	struct QuadEdge *qe = (struct QuadEdge*) malloc(sizeof(struct QuadEdge));;
	subdivision_construct(s, qe);
	int i;
	printf("consructed subdivision\n");
	for (i = 0; i < numPoints; i++) {
		subdivision_insertSite(s, &points[i]);
		printf("inserted point %d\n", i);
	}
	return s;
}