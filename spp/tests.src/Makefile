CFLAGS=-Wall -pedantic -std=c11 -I../../xbnf/utils -L../../xbnf/lib -I../utils -L../lib
LIBS=-lutils -lspp

all: tv

tv: tv.c
		gcc $(CFLAGS) tv.c $(LIBS) -o $@

clean:
				rm -f *~ tv *.o pass.* fail.*





