IDIR = ../include
CCOMPILER = gcc
CFLAGS = -I$(IDIR)

OBJECTDIR = ./obj

LIBRARIES = -lm #Math library

_DEPENDENCIES = Delaunay.h Subdivision.h QuadEdge.h Edge.h Point.h Predicate.h
DEPENDENCIES = $(patsubst %,$(IDIR)/%,$(_DEPENDENCIES))

_OBJECTS = Main.o Delaunay.o Subdivision.o QuadEdge.o Edge.o Point.o Predicate.o;
OBJECTS = $(patsubst %, $(OBJECTDIR)/%,$(_OBJ))

$(OBJECTDIR)/%.o: %.c $(DEPENDENCIES)
		$(CCOMPILER) -c -o $@ $< $(CFLAGS)

Triangulate: $(OBJECTS)
	$(CCOMPILER) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJECTDIR)/*.o *~ core $(IDIR)/*~