#ifndef CG_POINTARRAYLIST
#define CG_POINTARRAYLIST

struct PointArrayList {
	int numPoints;
	struct Point *points;
	struct PointArrayList *next;
};


#endif /* CG_POINTARRAYLIST */