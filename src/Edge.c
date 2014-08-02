#include "Edge.h"
#include "Point.h"

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
	return &(edge->o);
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

void edge_setCoordinates(struct Edge *edge, struct Point origin, struct Point destination) {
	edge_setOrigin(edge, origin);
	edge_setDestination(edge, destination);
}

void edge_setOrigin(struct Edge *edge, struct Point origin) {
	edge->o.x = origin.x;
	edge->o.y = origin.y;
}

void edge_setNext(struct Edge *edge, struct Edge *next) {
	edge->next = next;
}

void edge_setDestination(struct Edge *edge, struct Point destination) {
	edge_sym(edge)->o.x = destination.x;
	edge_sym(edge)->o.y = destination.y;
}

void edge_setRot(struct Edge *edge, struct Edge *rot) {
	edge->rot = rot;
}

struct Edge * edge_sym(struct Edge *edge) {
	return edge_rot(edge_rot(edge));
}

int edge_isWall(struct Edge *edge) {
	return (point_compare(edge_orig(edge), edge_orig(edge_lNext(edge))) >= 0) && (point_compare(edge_orig(edge_lNext(edge)), edge_orig(edge_lPrev(edge))) > 0);
}