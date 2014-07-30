#ifndef CG_QUADEDGE_H
#define CG_QUADEDGE_H

#include 'Edge.h'

typedef struct {
	struct Edge *first;
} QuadEdge;

struct QuadEdge * quadEdge_construct();

struct Edge * quadEdge_connect(struct Edge *a, struct Edge *b);

void quadEdge_deleteEdge(struct Edge * e);

struct Edge * quadEdge_getFirst(struct QuadEdge *q);

int quadEdge_isWall(struct Edge * e);

struct Edge * quadEdge_makeEdge();

void quadEdge_splice(struct Edge *a, struct Edge *b);

void quadEdge_swap(struct Edge *e);

#endif /* CG_QUADEDGE_H */