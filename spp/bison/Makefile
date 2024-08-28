#
# THE STANDARD BISON MAKEFILE -- DO NOT CHANGE OR MOVE THIS FILE
# This file is used to clean, make, or run all Bison parsers
#
CFLAGS=-Wall -pedantic -std=c11 -I. -I../../xbnf/bittobyte
OFILES=gmr.tab.o lexer.o gmr_tb.o
PPDIR=../../xbnf/postprocessor
LIBS=-L../../xbnf/lib -lbittobyte
GCC=gcc

all:	gmr

# bison: side effect: generates gmr.xml
gmr.tab.c:	gmr.y
#						bison -d -t -v --report=all -x gmr.y
						bison -d -t -v -x gmr.y
						$(PPDIR)/postprocessor gmr.xml gmr.tab.c

%.o:	%.c
			$(GCC) $(CFLAGS) -c $<

# build parser from .o files
gmr:	$(OFILES)
			$(GCC) $(CFLAGS) $(OFILES) $(LIBS) -o gmr

run:	gmr
			gmrtest.sh

clean:
			rm -f *~ *.o gmr.tab.* gmr.output gmr.xml gmr






