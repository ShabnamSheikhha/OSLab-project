IDIR =include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm

type=

_DEPS = shab_queue.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = shab_queue.o simpleWebServer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -D$(type) -DDEBUG -c -o $@ $< $(CFLAGS)

install: $(OBJ)
	$(CC) -D$(type) -DDEBUG -pthread -o server $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o server *~ core $(INCDIR)/*~





