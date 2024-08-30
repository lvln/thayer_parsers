CFLAGS=-Wall -pedantic -std=c11 -I../../utils -L../../lib -I../utils -L../lib
LIBS=-lutils -lspp -lbittobyte

all: tv converttobyte

tv: tv.c
		gcc $(CFLAGS) tv.c $(LIBS) -o $@

converttobyte: converttobyte.c
							 gcc $(CFLAGS) converttobyte.c $(LIBS) -o $@

clean:
				rm -f *~ tv converttobyte *.o pass.* fail.*





