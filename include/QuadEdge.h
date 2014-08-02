#ifndef CG_QUADEDGE_H
#define CG_QUADEDGE_H

#include "Point.h"
#include "Edge.h"

struct QuadEdge {
	struct Edge *first;
};

struct QuadEdge * quadEdge_construct(struct Point *bboxLL, struct Point *bboxUR);

struct Edge * quadEdge_connect(struct Edge *a, struct Edge *b);

void quadEdge_deleteEdge(struct Edge *edge);

int quadEdge_isWall(struct Edge *edge);

struct Edge * quadEdge_makeEdge();

void quadEdge_splice(struct Edge *a, struct Edge *b);

void quadEdge_swap(struct Edge *edge);

#endif /* CG_QUADEDGE_H */