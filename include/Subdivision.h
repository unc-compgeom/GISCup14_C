#ifndef CG_SUBDIVISION_H
#define CG_SUBDIVISION_H

#include "Point.h"
#include "Edge.h"
#include "QuadEdge.h"

struct Subdivision{
	struct Edge *startingEdge;
	struct QuadEdge *qe;
};

void subdivision_construct(struct Subdivision *s, struct QuadEdge *qe);

void subdivision_insertSite(struct Subdivision *s, struct Point *p);

struct Edge * subdivision_locate(struct Subdivision *s, struct Point *q);

#endif /* CG_SUBDIVISION_H */