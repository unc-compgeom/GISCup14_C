#include "Point.h"

int point_compare(const void *point1, const void *point2) {
	return 	((struct Point *) point1)->x < ((struct Point *) point2)->x ? -1 :
			((struct Point *) point1)->x > ((struct Point *) point2)->x ?  1 :
			((struct Point *) point1)->y < ((struct Point *) point2)->y ? -1 :
			((struct Point *) point1)->y > ((struct Point *) point2)->y ?  1 :
			0;
}