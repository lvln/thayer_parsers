CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=gmr.tab.o lexer.o gmr_tb.o
TABDIR=../../xbnf_skip_byte/tables

all:				gmr

# bison: generate gmr.tab.[ch]
gmr.tab.o:  gmr.y
#						bison -d -t -v --report=all -x gmr.y
						bison -d -t -v -x --debug gmr.y
						$(TABDIR)/addrule gmr.xml gmr.tab.c
						gcc $(CFLAGS) -c gmr.tab.c

gmr_tb.o:		gmr_tb.c
						gcc $(CFLAGS) -c gmr_tb.c

lexer.o:		lexer.c
						gcc $(CFLAGS) -c lexer.c

# combine parser, lexer, and main
gmr:				$(OFILES)
						gcc $(CFLAGS) $(OFILES) -o gmr

run:
						gmrtest.sh

clean:
						rm -f *~ *.o gmr.tab.* gmr.output gmr.xml gmr





