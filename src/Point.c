#include "Point.h"

int point_compare(struct Point * point1, struct Point * point2) {
	return 	point1->x < point2->x ? -1 :
			point1->x > point2->x ?  1 :
			point1->y < point2->y ? -1 :
			point1->y > point2->y ?  1 :
			0;
}

int point_compar(const void *p1, const void *p2) {
	struct Point *point1 = (struct Point *) p1;
	struct Point *point2 = (struct Point *) p2;
	return point_compare(point1, point2);
}