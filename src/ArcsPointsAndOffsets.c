#include "ArcsPointsAndOffsets.h"

struct ArcsPointsAndOffsets * arcsPointsAndOffsets_construct() {
	struct ArcsPointsAndOffsets *data = (struct ArcsPointsAndOffsets*) malloc(sizeof(struct ArcsPointsAndOffsets));
	return data;
}