#include "Predicate.h"

int predicate_ahead(struct Point *p, struct Point *q, struct Point *r) { // is p ahead of qr
	double prx = p->x - r->x;
	double pry = p->y - r->y;
	double rqx = r->x - q->x;
	double rqy = r->y - q->y;
	double dot = prx * rqx + pry * rqy;
	return dot > 0;	
}

double predicate_distSquared(struct Point *p, struct Point *q) {
	double dx = p->x - q->x, dy = p->y - q->y;
	return dx * dx + dy * dy;
}

int predicate_edgeIsPartOfRing(struct Edge *test, struct Edge *fromOrigin) {
	struct Edge *e;
	e = fromOrigin;
	do {
		if (e == test || edge_lNext(e) == test || edge_lNext(edge_lNext(e)) == test) {
			return 1;
		}
		e = edge_oNext(e);
	} while (e != fromOrigin);
	return 0;
}

int predicate_isPointInCircle(struct Point *test, struct Point *a,
			struct Point *b, struct Point *c) {
	double ax = a->x, ay = a->y;
	double bx = b->x, by = b->y;
	double cx = c->x, cy = c->y;
	double dx = test->x, dy = test->y;
	double det = (ax * ax + ay * ay) * predicate_triArea(b, c, test)
			- (bx * bx + by * by) * predicate_triArea(a, c, test)
			+ (cx * cx + cy * cy) * predicate_triArea(a, b, test)
			- (dx * dx + dy * dy) * predicate_triArea(a, b, c);
	return det > 0;
}

int predicate_leftOrAhead(struct Point *p, struct Point *q,
			struct Point *r) {
	double tmp = predicate_triArea(p, q, r);
	return tmp > 0 || tmp == 0 && predicate_ahead(p, q, r);
}

int predicate_onEdge(struct Point *p, struct Edge *e) {
		struct Point *a = edge_orig(e);
		struct Point *b = edge_dest(e);
	if (predicate_triArea(a, b, p) == 0) {
		double pax = p->x - a->x;
		double pay = p->y - a->y;
		double bax = b->x - a->x;
		double bay = b->y - a->y;
		double dot = pax * bax + pay * bay;
		double distSq = predicate_distSquared(a, b);
		return 0 <= dot && dot <= distSq;
	} else {
		return 0;
	}
}

int predicate_rightOf(struct Point *p, struct Edge *e) {
	return predicate_triArea(p, edge_orig(e), edge_dest(e)) < 0;
}

int predicate_rightOrAhead(struct Point *p, struct Point *q,
		struct Point *r) {
	double tmp = predicate_triArea(p, q, r);
	return tmp < 0 || tmp == 0 && predicate_ahead(p, q, r);
}
double predicate_triArea(struct Point *a, struct Point *b, struct Point *c) {
	double bax = b->x - a->x;
	double cay = c->y - a->y;
	double bay = b->y - a->y;
	double cax = c->x - a->x;
	return bax * cay - bay * cax; // det
}