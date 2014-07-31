INCLUDEDIR =../include
CCOMPILER=gcc
CFLAGS = -I$(INCLUDEDIR) -g

OBJECTDIR=obj
LIBRARYDIR =../lib

LIBRARIES=-lm

_DEPENDENCIES = ArcsPointsAndOffsets.h PointArrayList.h PointList.h ImportGML.h Delaunay.h Subdivision.h QuadEdge.h Edge.h Point.h Predicate.h
DEPENDENCIES = $(patsubst %,$(INCLUDEDIR)/%,$(_DEPENDENCIES))

_OBJECTS = ImportGML.o Main.o Delaunay.o Subdivision.o QuadEdge.o Edge.o Point.o Predicate.o;
OBJECTS = $(patsubst %,$(OBJECTDIR)/%,$(_OBJECTS))


$(OBJECTDIR)/%.o: %.c $(DEPENDENCIES)
	$(CCOMPILER) -c -o $@ $< $(CFLAGS)

simplify: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS) $(LIBRARIES)

.PHONY: clean

clean:
	rm -f $(OBJECTDIR)/*.o *~ core $(INCDIR)/*~ 