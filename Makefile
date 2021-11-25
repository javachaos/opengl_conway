IDIR=./include
CC=gcc
CFLAGS=-I$(IDIR) -O3 -mrdrnd -lglut -lGLU -lGL -fopenmp -lcunit

ODIR=./src/obj
LDIR=./lib
SRC=./src
TEST=./tests
ODIR_TEST=./tests/obj
LIBS=-lm

_DEPS = binary_matrix.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o binary_matrix.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ_TEST = test_main.o
OBJ_TEST = $(patsubst %,$(ODIR_TEST)/%,$(_OBJ_TEST))

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gameoflife: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR_TEST)/%.o: $(TEST)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ_TEST)
	$(CC) -o $@ $^ $(ODIR)/binary_matrix.o $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(ODIR_TEST)/*.o *~ core $(INCDIR)/*~
	rm -f test
