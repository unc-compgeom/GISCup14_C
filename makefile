INCLUDEDIR = ./include
CCOMPILER = gcc
CFLAGS = -I$(INCLUDEDIR)

OBJECTDIR = ./obj

LIBRARIES = -lm #Math library

_DEPENDENCIES = Delaunay.h Subdivision.h QuadEdge.h Edge.h Point.h Predicate.h
DEPENDENCIES = $(patsubst %,$(INCLUDEDIR)/%,$(_DEPENDENCIES))

_OBJECTS = Main.o Delaunay.o Subdivision.o QuadEdge.o Edge.o Point.o Predicate.o;
OBJECTS = $(patsubst %, $(OBJECTDIR)/%,$(_OBJ))

$(OBJECTDIR)/%.o: %.c $(DEPENDENCIES)
		$(CCOMPILER) -c -o $@ $< $(CFLAGS)

Triangulate: $(OBJECTS)
	$(CCOMPILER) -o $@ $^ $(CFLAGS) $(LIBRARIES)

.PHONY: clean

clean:
	rm -f $(OBJECTDIR)/*.o *~ core $(INCLUDEDIR)/*~