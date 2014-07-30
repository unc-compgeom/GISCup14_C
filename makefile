HEADERDIR = ./h
CCOMPILER = gcc
CFLAGS = -I$(HEADERDIR)

OBJECTDIR = ./obj
CDIR = ./c

_DEPENDENCIES = Delaunay.h Subdivision.h QuadEdge.h Edge.h Point.h Predicate.h
DEPENDENCIES + $(patsubst %,$(HEADERDIR)/%,$(_DEPENDENCIES))

_OBJECTS = Delaunay.o Subdivision.o QuadEdge.o Edge.o Point.o Predicate.o;
OBJECTS = $(patsubst %, $(OBJECTDIR)/%,$(_OBJ))

$(OBJECTDIR)/%.o: %.c $(DEPENDENCIES)
		$(CCOMPILER) -c -o $@ $< $(CFLAGS)

Triangulate: $(OBJECTS)
	$(CCOMPILER) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJECTDIR)/*.o *~ core $(HEADERDIR)/*~