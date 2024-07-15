CFLAGS=-Wall -pedantic -std=c11 -I.

all:			tv

tv.o:			tv.c mavlink.h
					gcc $(CFLAGS) -c tv.c

tv:				tv.o mavlink.o
					gcc $(CFLAGS) tv.o mavlink.o -o tv

mavlink.o: mavlink.c mavlink.h
					 gcc $(CFLAGS) -c mavlink.c

clean:
				rm -f *~ tv *.o





