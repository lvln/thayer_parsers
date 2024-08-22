CFLAGS=-Wall -pedantic -std=c11 -I../../mavlink/utils -L../../mavlink/lib
LIBS=-lutils

all: tv tvsingle

tv: tv.c
		gcc $(CFLAGS) tv.c $(LIBS) -o $@

tvsingle: tvsingle.c
					gcc $(CFLAGS) tvsingle.c $(LIBS) -o $@

clean:
				rm -f *~ tv tvsingle *.o pass.* fail.*





