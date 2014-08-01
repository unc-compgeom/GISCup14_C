#ifndef CG_POINT_H
#define CG_POINT_H

struct Point {
	double x;
	double y;
	int id;
};

int point_compare(const void *p1, const void *p2);

#endif /* CG_POINT_H */