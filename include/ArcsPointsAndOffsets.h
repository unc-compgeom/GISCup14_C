#ifndef CG_ARCSPOINTSANDOFFSETS_H
#define CG_ARCSPOINTSANDOFFSETS_H

struct ArcsPointsAndOffsets {
	struct PointsList *arcs;
	struct PointsList *points;
	long offsetLatitude;
	long offsetLongitude;
};

struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct();

#endif /* CG_ARCSPOINTSANDOFFSETS_H */