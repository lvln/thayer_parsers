CFLAGS=-Wall -pedantic -std=c11 -I../../mavlink/utils -L../../mavlink/lib
LIBS=-lutils

all: tv tvshort

tv: tv.c
		gcc $(CFLAGS) tv.c $(LIBS) -o $@

tvshort: tvshort.c
				 gcc $(CFLAGS) tvshort.c $(LIBS) -o $@

clean:
				rm -f *~ tv tvshort *.o pass.* fail.*





