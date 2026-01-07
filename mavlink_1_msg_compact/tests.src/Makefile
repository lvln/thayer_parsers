CFLAGS=-Wall -pedantic -std=c11 -I../../utils -L../../lib -I../../mavlink/utils -L../../mavlink/lib
LIBS=-lmavlink -lutils

all: tv

tv: tv.c
		gcc $(CFLAGS) tv.c $(LIBS) -o $@

clean:
				rm -f *~ tv  *.o pass.* fail.*





