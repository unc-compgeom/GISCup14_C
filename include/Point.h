#ifndef CG_POINT_H
#define CG_POINT_H

struct Point {
	double x;
	double y;
	int id;
};

int point_compare(struct Point * point1, struct Point * point2);

#endif /* CG_POINT_H */