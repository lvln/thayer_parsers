#
# Makefile for use when using xbnf_skip_byte preprocessor
#
CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=gmr.tab.o lexer.o gmr_tb.o
PPDIR=../../xbnf_skip_byte/postprocessor
GCC=clang

all:	gmr

# bison: side effect: generates gmr.xml
gmr.tab.c:	gmr.y
#						bison -d -t -v --report=all -x gmr.y
						bison -d -t -v -x gmr.y
						$(PPDIR)/addrule gmr.xml gmr.tab.c

%.o:	%.c
			$(GCC) $(CFLAGS) -c $<

# build parser from .o files
gmr:	$(OFILES)
			$(GCC) $(CFLAGS) $(OFILES) -o gmr

run:	gmr
			gmrtest.sh

clean:
			rm -f *~ *.o gmr.tab.* gmr.output gmr.xml gmr






