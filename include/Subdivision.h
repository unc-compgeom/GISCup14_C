#ifndef CG_SUBDIVISION_H
#define CG_SUBDIVISION_H
#include "Point.h"
#include "Edge.h"
#include "QuadEdge.h"

struct Subdivision {
	struct Edge *startingEdge;
	struct QuadEdge *qe;
};

struct Subdivision * subdivision_construct(struct Point *bboxLL, struct Point *bboxUR);

void subdivision_insertSite(struct Subdivision *s, struct Point *p);

struct Edge * subdivision_locate(struct Subdivision *s, struct Point *q);

struct Edge * subdivision_nextEdge(struct Subdivision *s, struct Edge *e);

#endif /* CG_SUBDIVISION_H */