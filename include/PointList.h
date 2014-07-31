#ifndef CG_POINTLIST_H
#define CG_POINTLIST_H

struct PointList {
	struct Point point;
	struct PointList *next;
};

#endif /* CG_POINTLIST_H */