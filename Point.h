#ifndef CG_POINT_H
#define CG_POINT_H

typedef struct {
	float x;
	float y;
} Point;

int point_compare(struct Point * point1, struct Point * point2);

#endif /* CG_POINT_H */