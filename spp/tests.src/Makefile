CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=spp.o bittobyte.o bit2byte.o vector.o

all:  $(OFILES) tv converttobyte bit2byte.d

%.o:	%.c %.h
			gcc $(CFLAGS) -c $<

tv:
			gcc $(CFLAGS) vector.o spp.o tv.c -o $@

converttobyte:
			gcc $(CFLAGS) bittobyte.o converttobyte.c -o $@

bit2byte.d:
			gcc $(CFLAGS) -DDEBUG bit2byte.c -o bit2byte.d

clean:
				rm -f *~ *.o pass.* fail.* bit2byte converttobyte tv





