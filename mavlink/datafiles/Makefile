CFLAGS=-Wall -pedantic -std=c11

all: read

read.o: read.c
				gcc $(CFLAGS) -c read.c

read: read.o
		  gcc $(CFLAGS) read.o -o read

clean:
			rm -f *.o read
