#ifndef CG_ARCSPOINTSANDOFFSETS_H
#define CG_ARCSPOINTSANDOFFSETS_H

struct ArcsPointsAndOffsets {
	struct PointsArrayList *arcs;
	struct PointsArrayList *points;
	long offsetLatitude;
	long offsetLongitude;
};

#endif /* CG_ARCSPOINTSANDOFFSETS_H */