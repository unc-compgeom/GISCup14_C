#include "QuadEdge.h"
#include "Edge.h"
#include "Point.h"
#include "Null.h"

void quadEdge_construct(struct QuadEdge *qe) {
	// initialize the triangle
	int scale;
	scale = 536870912;
	struct Point a = {-1 * scale - 1, 2 * scale};
	struct Point b = {-1 * scale, -1 * scale};
	struct Point c = {2 * scale, -1 * scale};

	struct Edge *ea;
	quadEdge_makeEdge(ea);
	edge_setCoordinates(ea, &a, &b);

	struct Edge *eb;
	quadEdge_makeEdge(eb);
	edge_setCoordinates(eb, &b, &c);
	quadEdge_splice(edge_sym(ea), eb);

	struct Edge *ec;
	quadEdge_makeEdge(ec);
	edge_setCoordinates(ec, &c, &a);
	quadEdge_splice(edge_sym(eb), ec);

	quadEdge_splice(edge_sym(ec), ea);

	qe->first = ec;
}

struct Edge * quadEdge_connect(struct Edge *a, struct Edge *b) {
	struct Edge *e;
	quadEdge_makeEdge(e);
	edge_setCoordinates(e, edge_dest(a), edge_orig(b));
	quadEdge_splice(edge_sym(e), b);
	return e;
}

void quadEdge_deleteEdge(struct Edge *edge) {
	quadEdge_splice(edge, edge_oPrev(edge));
	quadEdge_splice(edge_sym(edge), edge_oPrev(edge_sym(edge)));
}

struct Edge * quadEdge_getFirst(struct QuadEdge *qe) {
	int count;
	count = 0;
	struct Edge *e = qe->first;
	do {
		if (0 == count++) {
			return e;
		}
		if (quadEdge_isWall(e) || quadEdge_isWall(edge_sym(e))) {
			e = edge_rPrev(e);
		} else {
			e = edge_oNext(e);
		}
	} while (e != qe->first);
	return NULL;
}

int quadEdge_isWall(struct Edge *edge) {
	return  point_compare(edge_orig(edge), edge_orig(edge_lNext(edge))) >= 0 &&
			point_compare(edge_orig(edge_lNext(edge)), edge_orig(edge_lPrev(edge))) > 0;
}

void quadEdge_makeEdge(struct Edge *edge) {
	struct Point p1 = {0, 0};
	struct Point p2 = {0, 0};
	struct Point p3 = {0, 0};
	struct Point p4 = {0, 0};

	struct Edge e2, e3, e4;

	edge_setOrigin(edge, &p1);
	edge_setOrigin(&e2, &p2);
	edge_setOrigin(&e3, &p3);
	edge_setOrigin(&e4, &p4);

	edge_setRot(edge, &e2);
	edge_setRot(&e2, &e3);
	edge_setRot(&e3, &e4);
	edge_setRot(&e4, edge);

	edge_setNext(edge, edge);
	edge_setNext(&e2, &e4);
	edge_setNext(&e3, &e3);
	edge_setNext(&e4, &e2);
	
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
	edge_setCoordinates(e, edge_dest(a), edge_dest(b));
	quadEdge_splice(e, a);
	quadEdge_splice(edge_sym(e), b);
	quadEdge_splice(e, edge_lNext(a));
	quadEdge_splice(edge_sym(e), edge_lNext(b));
}