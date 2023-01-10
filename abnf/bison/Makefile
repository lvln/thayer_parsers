CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=abnf.tab.o lexer.o abnf_tb.o

all:				abnf

# bison: generate abnf.tab.[ch]
abnf.tab.o:  abnf.y
#						bison -d -t -v --report=all -x abnf.y
						bison -d -t -v -x abnf.y
						gcc $(CFLAGS) -c abnf.tab.c

abnf_tb.o:	abnf_tb.c
						gcc $(CFLAGS) -c abnf_tb.c

lexer.o:		lexer.c
						gcc $(CFLAGS) -c lexer.c

# combine parser, lexer, and main
abnf:				$(OFILES)
						gcc $(CFLAGS) $(OFILES) -o abnf

run:
						./abnftest.sh

clean:
						rm -f *~ *.o abnf.tab.* *output* gmr.xml abnf



