CFLAGS=-Wall -pedantic -std=c11 -I../../xbnf/utils -I.
OFILES=spp.o

all:  $(OFILES)
			ar cr ../lib/libspp.a $(OFILES)

%.o:  %.c %.h
			gcc $(CFLAGS) -c $<

clean:
			rm -f *.o ../lib/libspp.a
