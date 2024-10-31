CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=bit2byte.o spp.o vector.o 

all:  $(OFILES) bit2byte.d tv

%.o:	%.c %.h
			gcc $(CFLAGS) -c $<

tv:		tv.c $(OFILES)
			gcc $(CFLAGS) spp.o vector.o tv.c -o tv

bit2byte.d:
			gcc $(CFLAGS) -DDEBUG bit2byte.c -o bit2byte.d

clean:
				rm -f *~ *.o pass.* fail.* bit2byte.d tv





