CFLAGS=-Wall -pedantic -std=c11 -I../../utils -I.
OFILES=mavlink.o

all:  $(OFILES)
			ar cr ../lib/libmavlink.a $(OFILES)

%.o:  %.c %.h
			gcc $(CFLAGS) -c $<

clean:
			rm -f *.o ../lib/libmavlink.a
