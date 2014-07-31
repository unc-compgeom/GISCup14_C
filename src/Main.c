#include <stdio.h>
#include <stdlib.h>
#include "Delaunay.h"
#include "Subdivision.h"
#include "ImportGML.h"

int main() {
	struct ArcsPointsAndOffsets *importedStuff;
	importedStuff = importGML_importGML("../td1/lines_out.txt", "../td1/points_out.txt");
	/*
	printf("TEST:\n");
	printf("Generate sample points\n");
	struct Point points[10];
	struct Point tmp1 	= 	{0,0};
	struct Point tmp2 	= 	{1,0};
	struct Point tmp3 	= 	{0,2};
	struct Point tmp4 	= 	{3,1};
	struct Point tmp5 	= 	{3,6};
	struct Point tmp6 	= 	{2,0};
	struct Point tmp7 	= 	{1,9};
	struct Point tmp8 	= 	{1,7};
	struct Point tmp9	= 	{2,4};
	struct Point tmp10 	= 	{80, 10};
	printf("  Made ten points\n");
	int i;
	i = 0;
	points[i++] = tmp1;
	points[i++] = tmp2;
	points[i++] = tmp3;
	points[i++] = tmp4;
	points[i++] = tmp5;
	points[i++] = tmp6;
	points[i++] = tmp7;
	points[i++] = tmp8;
	points[i++] = tmp9;
	points[i++] = tmp10;
	printf("  Inserted ten points\n");
	printf("Began triangulation\n");
	struct Subdivision *triangulation;
	triangulation = delaunay_triangulate(points, 10);
	*/
}