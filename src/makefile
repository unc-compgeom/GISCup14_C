INCLUDEDIR =../include
CCOMPILER=gcc
CFLAGS = -I$(INCLUDEDIR) -g -std=c99

OBJECTDIR=obj
LIBRARYDIR =../lib

LIBRARIES=-lm

_DEPENDENCIES = ExportGML.h ArcsPointsAndOffsets.h Delaunay.h Edge.h ExportGML.h ImportGML.h Point.h PointArrayList.h PointList.h Predicate.h QuadEdge.h Subdivision.h UniquePointList.h
DEPENDENCIES = $(patsubst %,$(INCLUDEDIR)/%,$(_DEPENDENCIES))

_OBJECTS = ArcsPointsAndOffsets.o Delaunay.o Edge.o ExportGML.o ImportGML.o Main.o Point.o Predicate.o QuadEdge.o Subdivision.o
OBJECTS = $(patsubst %,$(OBJECTDIR)/%,$(_OBJECTS))


$(OBJECTDIR)/%.o: %.c $(DEPENDENCIES)
	$(CCOMPILER) -c -o $@ $< $(CFLAGS)

Simplify.exe: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS) $(LIBRARIES)

.PHONY: clean

clean:
	rm -f $(OBJECTDIR)/*.o *~ core $(INCDIR)/*~ 