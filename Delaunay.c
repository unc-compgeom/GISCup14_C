#include "Point.h"
#include "Edge.h"
#include "QuadEdge.h"
#include "Subdivision.h"
#include "Predicate.h"
#include "Delaunay.h"

///////////////// POINT /////////////////
int point_compare(struct Point * point1, struct Point * point2) {
	return 	point1->x < point2->x ? -1 :
			point1->x > point2->x ?  1 :
			point1->y < point2->y ? -1 :
			point1->y > point2->y ?  1 :
			0;
}

///////////////// EDGE /////////////////
struct Point * edge_dest(struct Edge *edge) {
	return edge_orig(edge_sym(edge));
}

struct Edge * edge_dNext(struct Edge *edge) {
	return edge_sym(edge_oNext(edge_sym(edge)));
}

struct Edge * edge_dPrev(struct Edge *edge) {
	return edge_invRot(edge_oNext(edge_invRot(edge)));
}

struct Edge * edge_invRot(struct Edge *edge) {
	return edge_rot(edge_rot(edge_rot(edge)));
}

struct Edge * edge_lNext(struct Edge *edge) {
	return edge_rot(edge_oNext(edge_invRot(edge)));
}

struct Edge * edge_lPrev(struct Edge *edge) {
	return edge_sym(edge_oNext(edge));
}

struct Edge * edge_oNext(struct Edge *edge) {
	return edge->next;
}

struct Edge * edge_oPrev(struct Edge *edge) {
	return edge_rot(edge_oNext(edge_rot(edge)));
}

struct Point * edge_orig(struct Edge *edge) {
	return edge->o;
}

struct Edge * edge_rNext(struct Edge *edge) {
	return edge_invRot(edge_oNext(edge_rot(edge)));
}

struct Edge * edge_rot(struct Edge *edge) {
	return edge->rot;
}

struct Edge * edge_rPrev(struct Edge *edge) {
	return edge_oNext(edge_sym(edge));
}

void edge_setCoordinates(struct Edge *edge, struct Point * origin, struct Point * destination) {
	edge_setOrigin(edge, origin);
	edge_setDestination(edge, destination);
}

void edge_setOrigin(struct Edge *edge, struct Point * origin) {
	edge->o = origin;
}

void edge_setNext(struct Edge *edge, struct Edge * next) {
	edge->next = next;
}

void edge_setDestination(struct Edge *edge, struct Point * destination) {
	edge_sym(edge)->o = destination;
}

void edge_setRot(struct Edge *edge, struct Edge * rot) {
	edge->rot = rot;
}

struct Edge * edge_sym(struct Edge *edge) {
	return edge_rot(edge_rot(edge));
}

///////////////// QUADEDGE /////////////////
struct QuadEdge * quadEdge_construct() {
	// initialize the triangle
	int scale;
	scale = 536870912;
	struct Point a = {-1 * scale - 1, 2 * scale};
	struct Point b = {-1 * scale, -1 * scale};
	struct Point c = {2 * scale, -1 * scale};

	struct Edge *ea = quadEdge_makeEdge();
	edge_setCoordinates(ea, &a, &b);

	struct Edge *eb = quadEdge_makeEdge();
	edge_setCoordinates(&eb, &b, &c);
	quadEdge_splice(edge_sym(ea), eb);

	struct Edge *ec = quadEdge_makeEdge();
	edge_setCoordinates(&ec, &c, &a);
	quadEdge_splice(edge_sym(eb), ec);

	quadEdge_splice(edge_sym(ec), ea);

	struct QuadEdge qe = {&ec};
	return &qe;
}

struct Edge * quadEdge_connect(struct Edge *a, struct Edge *b) {
	struct Edge *e = quadEdge_makeEdge();
	edge_setCoordinates(e, edge_dest(a), edge_orig(b));
	quadEdge_splice(edge_sym(e), b);
	return e;
}

void quadEdge_deleteEdge(struct Edge * e) {
	quadEdge_splice(e, edge_oPrev(e));
	quadEdge_splice(edge_sym(e), edge_oPrev(edge_sym(e)));
}

struct Edge * quadEdge_getFirst(struct QuadEdge *q) {
	int count;
	count = 0;
	struct Edge *e = q->first;
	do {
		if (0 == count++) {
			return e;
		}
		if (quadEdge_isWall(e) || quadEdge_isWall(edge_sym(e))) {
			e = edge_rPrev(e);
		} else {
			e = edge_oNext(e);
		}
	} while (e != q->first);
	return NULL;
}

int quadEdge_isWall(struct Edge * e) {
	return  point_compare(edge_orig(e), edge_orig(edge_lNext(e))) >= 0 &&
			point_compare(edge_orig(edge_lNext(e)), edge_orig(edge_lPrev(e))) > 0;
}

struct Edge * quadEdge_makeEdge() {
	struct Point p1 = {0, 0};
	struct Point p2 = {0, 0};
	struct Point p3 = {0, 0};
	struct Point p4 = {0, 0};

	struct Edge e1, e2, e3, e4;

	edge_setOrigin(&e1, &p1);
	edge_setOrigin(&e2, &p2);
	edge_setOrigin(&e3, &e3);
	edge_setOrigin(&e4, &e4);

	edge_setRot(&e1, &e2);
	edge_setRot(&e2, &e3);
	edge_setRot(&e3, &e4);
	edge_setRot(&e4, &e1);

	edge_setNext(&e1, &e1);
	edge_setNext(&e2, &e4);
	edge_setNext(&e3, &e3);
	edge_setNext(&e4, &e2);
	
	return &e1;
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

///////////////// SUBDIVISION /////////////////
struct Subdivision * subdivision_construct() {
	QuadEdge *qe = quadEdge_construct();
	struct Edge *startingEdge = quadEdge_getFirst(qe);
	Subdivision s = {startingEdge, qe};
}

void subdivision_insertSite(struct Subdivision *s, struct Point *p) {
	struct Edge *e = subdivision_locate(p);
	if (predicate_onEdge(p, e)) {
		e = edge_oPrev(e);
		quadEdge_deleteEdge(edge_oNext(e));
	}
	struct Edge *base = quadEdge_makeEdge();
	struct Point tmpDest = {p->x, p->y};
	edge_setCoordinates(base, edge_orig(e), &tmpDest);
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
		} else if (edge_oNext(e) == startingEdge) {
			return;
		} else {
			e = edge_lPrev(edge_oNext(e));
		}
	} while (1);
}

struct Edge * locate(struct Subdivision *s, struct Point *q) {
	struct Edge *e = s->startingEdge;
	if (!preciate_rightOrAhead(edge_dest(e), edge_orig(e), q)) {
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

///////////////// PREDICATE /////////////////
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
double triArea(struct Point *a, struct Point *b, struct Point *c) {
	float bax = b->x - a->x;
	float cay = c->y - a->y;
	float bay = b->y - a->y;
	float cax = c->x - a->x;
	return bax * cay - bay * cax; // det
}

///////////////// DELAUNAY TRIANGULATION /////////////////
struct Subdivision * triangulate(struct Point points[], int numPoints) {
	struct Subdivision *s = subdivision_construct();
	int i;
	for (i = 0; i < numPoints; i++) {
		subdivision_insertSite(s, &points[i]);
	}
	return s;
}