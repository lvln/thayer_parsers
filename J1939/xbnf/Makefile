CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=gmr.tab.o lexer.o gmr_tb.o
XBNFDIR=../../xbnf/bison

all:		gmr

gmr.y:  	gmr.xbnf
			$(XBNFDIR)/xbnf gmr.xbnf -o gmr.y

# bison: generate gmr.tab.[ch]
gmr.tab.c:	gmr.y
#			bison -d -t -v --report=all -x gmr.y
			bison -d -t -v -x gmr.y

%.o:		%.c
			gcc $(CFLAGS) -c $<

# combine parser, lexer, and main
gmr:		$(OFILES)
			gcc $(CFLAGS) $(OFILES) -o gmr

run:		gmr
			gmrtest.sh

clean:
			rm -f *~ *.o gmr.tab.* gmr.output gmr.xml gmr.y gmr gmr_counts





