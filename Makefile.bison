#
# THE STANDARD BISON MAKEFILE -- DO NOT CHANGE OR MOVE THIS FILE
# This file is used to clean, make, or run all Bison parsers
#
CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=gmr.tab.o lexer.o gmr_tb.o

all:	gmr

# bison: side effect: generates gmr.xml
gmr.tab.c:	gmr.y
#						bison -d -t -v --report=all -x gmr.y
						bison -d -t -v -x gmr.y

%.o:	%.c
			gcc $(CFLAGS) -c $<

# build parser from .o files
gmr:	$(OFILES)
			gcc $(CFLAGS) $(OFILES) -o gmr

run:	gmr
			gmrtest.sh

clean:
			rm -f *~ *.o gmr.tab.* gmr.output gmr.xml gmr






