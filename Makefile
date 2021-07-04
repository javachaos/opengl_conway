IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) -O3 -mrdrnd -lglut -lGLU -lGL -fopenmp

ODIR=./src/obj
LDIR=./lib
SRC=./src/
LIBS=-lm

_DEPS = binary_matrix.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o binary_matrix.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gameoflife: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
