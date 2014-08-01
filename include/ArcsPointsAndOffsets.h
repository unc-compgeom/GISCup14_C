#ifndef CG_ARCSPOINTSANDOFFSETS_H
#define CG_ARCSPOINTSANDOFFSETS_H

struct ArcsPointsAndOffsets {
	struct PointArrayList *arcs;
	int arcPointCount;
	struct PointArrayList *points;
	long offsetLongitude;
	long offsetLatitude;
};

#endif /* CG_ARCSPOINTSANDOFFSETS_H */