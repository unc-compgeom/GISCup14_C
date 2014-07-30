#ifndef CG_SUBDIVISION_H
#define CG_SUBDIVISION_H

#include 'Point.h'
#include 'Edge.h'

typedef struct {
	struct Edge *startingEdge;
	struct QuadEdge *qe;
} Subdivision;

struct Subdivision * subdivision_construct();

void subdivision_insertSite(struct Subdivision *s, struct Point *p);

struct Edge * locate(struct Subdivision *s, struct Point *q);

#endif /* CG_SUBDIVISION_H */