CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=mavlink.o vector.o

all:  $(OFILES)
			ar cr ../lib2/libutils.a $(OFILES)

%.o:  %.c %.h
			gcc $(CFLAGS) -c $<

clean:
			rm -f *.o ../lib2/libutils.a
