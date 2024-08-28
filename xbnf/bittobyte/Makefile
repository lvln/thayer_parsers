CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=bittobyte.o

all:  $(OFILES)
			ar cr ../lib/libbittobyte.a $(OFILES)

%.o:  %.c %.h
			gcc $(CFLAGS) -c $<

clean:
			rm -f *.o ../lib/libbittobyte.a
