#include "Point.h"

int point_compare(struct Point * point1, struct Point * point2) {
	return 	point1->x < point2->x ? -1 :
			point1->x > point2->x ?  1 :
			point1->y < point2->y ? -1 :
			point1->y > point2->y ?  1 :
			0;
}
