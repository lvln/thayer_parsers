CFLAGS=-Wall -pedantic -std=c11

all: read cleandata

read.o: read.c
				gcc $(CFLAGS) -c read.c

read: read.o
		  gcc $(CFLAGS) read.o -o read

cleandata.o: cleandata.c
				gcc $(CFLAGS) -c cleandata.c

cleandata: cleandata.o
		  gcc $(CFLAGS) cleandata.o -o cleandata

clean:
			rm -f *.o read cleandata
