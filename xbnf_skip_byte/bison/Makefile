CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=xbnf.tab.o lexer.o xbnf_tb.o

all:				xbnf

# bison: generate xbnf.tab.[ch]
xbnf.tab.o:  xbnf.y
#						bison -d -t -v --report=all -x xbnf.y
						bison -d -t -v -x xbnf.y
						gcc $(CFLAGS) -c xbnf.tab.c

xbnf_tb.o:	xbnf_tb.c
						gcc $(CFLAGS) -c xbnf_tb.c

lexer.o:		lexer.c
						gcc $(CFLAGS) -c lexer.c

# combine parser, lexer, and main
xbnf:				$(OFILES)
						gcc $(CFLAGS) $(OFILES) -o xbnf

run:
						./xbnftest.sh

clean:
						rm -f *~ *.o xbnf.tab.* *output* *xml xbnf



