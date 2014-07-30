#include <stdio>
#include "Delaunay.h"
#include "Subdivision.h"

int main() {
	printf("test");
	struct Subdivision *triangulation;
	struct Point points[10];
	triangulation = delaunay_triangulate(points, 10);
}