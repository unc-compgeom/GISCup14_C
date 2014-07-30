#include "Predicate.h"
#include "Point.h"
#include "Edge.h"

int predicate_ahead(struct Point *p, struct Point *q, struct Point *r) {
	float pqx = p->x - q->x;
	float pqy = p->y - q->y;
	float rqx = r->x - q->x;
	float rqy = r->y - q->y;
	float dot = pqx * rqx + pqy * rqy;
	return dot > predicate_distSquared(q, r);
}

double predicate_distSquared(struct Point *p, struct Point *q) {
	double dx = p->x - q->x, dy = p->y - q->y;
	return dx * dx + dy * dy;
}

int predicate_isPointInCircle(struct Point *test, struct Point *a,
			struct Point *b, struct Point *c) {
	float ax = a->x, ay = a->y;
	float bx = b->x, by = b->y;
	float cx = c->x, cy = c->y;
	float dx = test->x, dy = test->y;
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
		float pax = p->x - a->x;
		float pay = p->y - a->y;
		float bax = b->x - a->x;
		float bay = b->y - a->y;
		float dot = pax * bax + pay * bay;
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
	float bax = b->x - a->x;
	float cay = c->y - a->y;
	float bay = b->y - a->y;
	float cax = c->x - a->x;
	return bax * cay - bay * cax; // det
}