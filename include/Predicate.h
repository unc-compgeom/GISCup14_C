#ifndef CG_PREDICATE_H
#define CG_PREDICATE_H

#include "Point.h"
#include "Edge.h"

int predicate_ahead(struct Point *p, struct Point *q, struct Point *r);

double predicate_distSquared(struct Point *p, struct Point *q);

int predicate_edgeIsPartOfRing(struct Edge *test, struct Edge *fromOrigin);

int predicate_isPointInCircle(struct Point *test, struct Point *a, struct Point *b, struct Point *c);

int predicate_leftOrAhead(struct Point *p, struct Point *q,	struct Point *r);

int predicate_onEdge(struct Point *p, struct Edge *e);

int predicate_rightOf(struct Point *p, struct Edge *e);

int predicate_rightOrAhead(struct Point *p, struct Point *q, struct Point *r);

double predicate_triArea(struct Point *a, struct Point *b, struct Point *c);

#endif /* CG_PREDICATE_H */