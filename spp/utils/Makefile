CFLAGS=-Wall -pedantic -std=c11 -I../../utils -I.
OFILES=spp.o bittobyte.o

all:  $(OFILES)
			ar cr ../lib/libspp.a spp.o
			ar cr ../lib/libbittobyte.a bittobyte.o

%.o:  %.c %.h
			gcc $(CFLAGS) -c $<

clean:
			rm -f *.o ../lib/libspp.a ../lib/libbittobyte.a
