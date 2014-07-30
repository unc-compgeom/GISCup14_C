#ifndef CG_EDGE_H
#define CG_EDGE_H


#include "Point.h"

struct Edge {
	struct Point *o;
	struct Edge *next;
	struct Edge *rot;
};

struct Point * edge_dest(struct Edge *edge);

struct Edge * edge_dNext(struct Edge *edge);

struct Edge * edge_dPrev(struct Edge *edge);

struct Edge * edge_invRot(struct Edge *edge);

struct Edge * edge_lNext(struct Edge *edge);

struct Edge * edge_lPrev(struct Edge *edge);

struct Edge * edge_oNext(struct Edge *edge);

struct Edge * edge_oPrev(struct Edge *edge);

struct Point * edge_orig(struct Edge *edge);

struct Edge * edge_rNext(struct Edge *edge);

struct Edge * edge_rot(struct Edge *edge);

struct Edge * edge_rPrev(struct Edge *edge);

struct Edge * edge_rNext(struct Edge *edge);

void edge_setCoordinates(struct Edge *edge, struct Point *origin, 
		struct Point * destination);

void edge_setOrigin(struct Edge *edge, struct Point *origin);

void edge_setNext(struct Edge *edge, struct Edge *next);

void edge_setDestination(struct Edge *edge, struct Point * estination);

void edge_setRot(struct Edge *edge, struct Edge * ot);

struct Edge * edge_sym(struct Edge *edge);

#endif /* CG_EDGE_H */