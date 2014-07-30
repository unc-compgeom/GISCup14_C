#ifndef CG_QUADEDGE_H
#define CG_QUADEDGE_H

#include "Edge.h"

struct QuadEdge {
	struct Edge *first;
};

void quadEdge_construct(struct QuadEdge *qe);

struct Edge * quadEdge_connect(struct Edge *a, struct Edge *b);

void quadEdge_deleteEdge(struct Edge *edge);

struct Edge * quadEdge_getFirst(struct QuadEdge *q);

int quadEdge_isWall(struct Edge *edge);

void quadEdge_makeEdge(struct Edge *edge);

void quadEdge_splice(struct Edge *a, struct Edge *b);

void quadEdge_swap(struct Edge *edge);

#endif /* CG_QUADEDGE_H */