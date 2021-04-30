HFILES= parser.h pp_lalr_xml.h 
OFILES= main.o parser.o pp_lalr.o
CFLAGS= -Wall -pedantic -std=c11 \
				-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include\
				-I/usr/local/include -L/usr/local/lib

all:		hmr

%.o:		%.c $(HFILES)
				gcc $(CFLAGS) -c $<

hmr:		$(OFILES)
				gcc $(CFLAGS) $(OFILES) -o hmr -lhammer -lglib-2.0

xml:	
				./hmr -g

run:
				./gmrtest.sh

clean:
				rm -f *~ *.o *.xml hmr hmr.output




