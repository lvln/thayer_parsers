CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=spp.o vector.o 

all:  $(OFILES) tv

%.o:	%.c %.h
			gcc $(CFLAGS) -c $<

tv:		tv.c $(OFILES)
			gcc $(CFLAGS) spp.o vector.o tv.c -o tv

clean:
	rm -rf *~ *.o pass.* fail.* tv *.dSYM





