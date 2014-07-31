#ifndef CG_POINTLIST_H
#define CG_POINTLIST_H

struct PointList {
	double x;
	double y;
	struct PointList *next;
};

#endif /* CG_POINTLIST_H */