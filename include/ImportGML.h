#ifndef CG_IMPORTGML_H
#define CG_IMPORTGML_H

struct ArcsPointsAndOffsets {
	struct PointsArrayList *arcs;
	struct PointsArrayList *points;
	long offsetLatitude;
	long offsetLongitude;
};

struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct();

struct PointsList {
	double x;
	double y;
	struct PointsList *next;
};

struct PointsArrayList {
	int numPoints;
	struct Point *points;
	struct PointsArrayList *next;
};

struct PointsArrayList * importGML_readFile(char fileName[]);

struct ArcsPointsAndOffsets * importGML_importGML(char arcsFilename[], char pointsFilename[]);

#endif /* #ifndef CG_IMPORTGML_H */