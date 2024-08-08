CFLAGS=-Wall -pedantic -std=c11 -I../../mavlink/utils -L../../mavlink/lib
LIBS=-lutils

all: addrule

addrule: addrule.c
				 gcc $(CFLAGS) addrule.c $(LIBS) -o $@

clean:
			rm -f *~ addrule *.o
