#include "QuadEdge.h"
#include "Edge.h"
#include "Point.h"
#include "Null.h"
#include <stdlib.h> 

struct QuadEdge * quadEdge_construct() {
	struct QuadEdge *qe = (struct QuadEdge*) malloc(sizeof(struct QuadEdge));
	// initialize the triangle
	struct Point *a = (struct Point*) malloc(sizeof(struct Point));
	struct Point *b = (struct Point*) malloc(sizeof(struct Point));
	struct Point *c = (struct Point*) malloc(sizeof(struct Point));

	int scale;
	scale = 536870912;
	a->x = -1 * scale - 1;
	a->y = 2 * scale;
	b->x = -1 * scale;
	b->y = -1 * scale;
	c->x = 2 * scale;
	c->y = -1 * scale;

	struct Edge *ea;
	ea = quadEdge_makeEdge();
	struct Edge *eb;
	eb = quadEdge_makeEdge();
	struct Edge *ec;
	ec = quadEdge_makeEdge();

	edge_setCoordinates(ea, *a, *b);
	edge_setCoordinates(eb, *b, *c);
	edge_setCoordinates(ec, *c, *a);

	quadEdge_splice(edge_sym(ea), eb);
	quadEdge_splice(edge_sym(eb), ec);
	quadEdge_splice(edge_sym(ec), ea);

	qe->first = ec;

	free(a);
	free(b);
	free(c);

	return qe;
}

struct Edge * quadEdge_connect(struct Edge *a, struct Edge *b) {
	struct Edge *e;
	e = quadEdge_makeEdge();
	edge_setCoordinates(e, *edge_dest(a), *edge_orig(b));
	quadEdge_splice(edge_sym(e), b);
	return e;
}

void quadEdge_deleteEdge(struct Edge *edge) {
	quadEdge_splice(edge, edge_oPrev(edge));
	quadEdge_splice(edge_sym(edge), edge_oPrev(edge_sym(edge)));
}

int quadEdge_isWall(struct Edge *edge) {
	return  point_compare(edge_orig(edge), edge_orig(edge_lNext(edge))) >= 0 &&
			point_compare(edge_orig(edge_lNext(edge)), edge_orig(edge_lPrev(edge))) > 0;
}

struct Edge * quadEdge_makeEdge() {
	struct Edge *edge = (struct Edge*) malloc(sizeof(struct Edge));
	struct Edge *e2 = (struct Edge*) malloc(sizeof(struct Edge));
	struct Edge *e3 = (struct Edge*) malloc(sizeof(struct Edge));
	struct Edge *e4 = (struct Edge*) malloc(sizeof(struct Edge));

	edge_setRot(edge, e2);
	edge_setRot(e2, e3);
	edge_setRot(e3, e4);
	edge_setRot(e4, edge);

	edge_setNext(edge, edge);
	edge_setNext(e2, e4);
	edge_setNext(e3, e3);
	edge_setNext(e4, e2);

	return edge;
}

void quadEdge_splice(struct Edge *a, struct Edge *b) {
	struct Edge *alpha = edge_rot(edge_oNext(a));
	struct Edge *beta = edge_rot(edge_oNext(b));

	struct Edge *t1 = edge_oNext(b);
	struct Edge *t2 = edge_oNext(a);
	struct Edge *t3 = edge_oNext(beta);
	struct Edge *t4 = edge_oNext(alpha);

	edge_setNext(a, t1);
	edge_setNext(b, t2);
	edge_setNext(alpha, t3);
	edge_setNext(beta, t4);
}

void quadEdge_swap(struct Edge *e) {
	struct Edge *a = edge_oPrev(e);
	struct Edge *b = edge_oPrev(edge_sym(e));
	edge_setCoordinates(e, *edge_dest(a), *edge_dest(b));
	quadEdge_splice(e, a);
	quadEdge_splice(edge_sym(e), b);
	quadEdge_splice(e, edge_lNext(a));
	quadEdge_splice(edge_sym(e), edge_lNext(b));
}