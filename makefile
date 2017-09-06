IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=.
LDIR =.

#LIBS=-lm

#_DEPS = hellomake.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = doublell.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c #$(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

doublell: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) #$(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o 
