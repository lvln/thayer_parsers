CFLAGS=-Wall -pedantic -std=c11

all: read cleandata extractdata

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

clean:
			rm -f *.o read cleandata extractdata
