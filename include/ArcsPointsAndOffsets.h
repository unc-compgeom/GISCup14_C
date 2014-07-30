#ifndef CG_ARCSPOINTSANDOFFSETS_H
#define CG_ARCSPOINTSANDOFFSETS_H

struct ArcsPointsAndOffsets {
	struct PointsList *arcs;
	struct PointsList *points;
	long offsetLatitude;
	long offsetLongitude;
};

struct PointsList {
	struct PointsList *next;
	struct Point *points; // array
	int numPoints;
};

struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct();

#endif /* CG_ARCSPOINTSANDOFFSETS_H */