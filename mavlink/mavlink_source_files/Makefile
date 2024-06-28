CFLAGS=-Wall -pedantic -std=c11 -I.

all: read cleandata extractdata countcodes untruncate

mavlink.o: mavlink.c mavlink.h
					 gcc $(CFLAGS) -c mavlink.c

untruncate.o: untruncate.c mavlink.h
							gcc $(CFLAGS) -c  untruncate.c

untruncate: untruncate.o mavlink.o
						gcc $(CFLAGS) untruncate.o mavlink.o -o untruncate

read.o: read.c
				gcc $(CFLAGS) -c read.c

read: read.o
		  gcc $(CFLAGS) read.o -o read

cleandata.o: cleandata.c
				gcc $(CFLAGS) -c cleandata.c

cleandata: cleandata.o
		  gcc $(CFLAGS) cleandata.o -lm -o cleandata

extractdata.o: extractdata.c
				gcc $(CFLAGS) -c extractdata.c

extractdata: extractdata.o
		  gcc $(CFLAGS) extractdata.o -lm -o extractdata

countcodes.o: countcodes.c
				gcc $(CFLAGS) -c countcodes.c

countcodes: countcodes.o
		  gcc $(CFLAGS) countcodes.o -o countcodes

clean:
			rm -f *.o read cleandata extractdata countcodes untruncate
