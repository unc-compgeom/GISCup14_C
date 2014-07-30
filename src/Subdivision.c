#include "Subdivision.h"
#include "QuadEdge.h"
#include "Edge.h"
#include "Point.h"
#include "Predicate.h"
#include "Null.h"
#include <stdlib.h> 

void subdivision_construct(struct Subdivision *s, struct QuadEdge *qe) {
	struct QuadEdge *qe;
	qe = quadEdge_construct();
	struct Edge *startingEdge = (struct Edge*) malloc(sizeof(struct Edge));;
	startingEdge = quadEdge_getFirst(qe);
	s->startingEdge = startingEdge;
	s->qe = qe;
}

void subdivision_insertSite(struct Subdivision *s, struct Point *p) {
	struct Edge *e = (struct Edge*) malloc(sizeof(struct Edge));
	e = subdivision_locate(s, p);
	if (predicate_onEdge(p, e)) {
		e = edge_oPrev(e);
		quadEdge_deleteEdge(edge_oNext(e));
	}
	struct Edge *base;
	base = quadEdge_makeEdge();
	struct Point tmpDest = {p->x, p->y};
	edge_setCoordinates(base, *edge_orig(e), tmpDest);
	quadEdge_splice(base, e);
	s->startingEdge = base;
	do {
		base = quadEdge_connect(e, edge_sym(base));
		e = edge_oPrev(base);
	} while (edge_lNext(e) != s->startingEdge);
	do {
		struct Edge *t = edge_oPrev(e);
		if (predicate_rightOf(edge_dest(t), e) && predicate_isPointInCircle(p, edge_orig(e), edge_dest(t), edge_dest(e))) {
			quadEdge_swap(e);
			e = edge_oPrev(e);
		} else if (edge_oNext(e) == s->startingEdge) {
			return;
		} else {
			e = edge_lPrev(edge_oNext(e));
		}
	} while (1);
}

struct Edge * subdivision_locate(struct Subdivision *s, struct Point *q) {
	struct Edge *e = s->startingEdge;
	if (!predicate_rightOrAhead(edge_dest(e), edge_orig(e), q)) {
		e = edge_sym(e);
	}
	struct Point *p = edge_orig(e);
	if (p == q) {
		// duplicate point
		return NULL;
	}
	do {
		if (q == edge_dest(e)) {
			// duplicate point
			return NULL;
		} else if (!predicate_leftOrAhead(q, edge_orig(e), edge_dest(e))) {
			return edge_sym(e);
		} else if (predicate_rightOrAhead(edge_dest(edge_oNext(e)), p, q)) {
			e = edge_oNext(e);
		} else {
			e = edge_sym(edge_lNext(e));
		}
	} while (1);
}