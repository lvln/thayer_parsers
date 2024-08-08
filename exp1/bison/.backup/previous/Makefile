CGLAGS=-Wall -pedantic -std=c11 -I.

all:				gmr

# bison: generate gmr.tab.[ch]
gmr.tab.o:  gmr.y
#						bison -d -t -v --report=all -x gmr.y
						bison -d -t -v -x gmr.y
						gcc $(CFLAGS) -c gmr.tab.c

# flex: generate lexical analyser
lex.yy.o:		gmr.l
						flex gmr.l
						gcc $(CFLAGS) -c lex.yy.c

gmr_tb.o:
						gcc $(CFLAGS) -c gmr_tb.c

# combine parser, lexer, and main
gmr:				gmr.tab.o lex.yy.o gmr_tb.o
						gcc $(CFLAGS) lex.yy.o gmr.tab.o gmr_tb.o -o gmr

run:
						gmrtest.sh

clean:
						rm -f *~ *.o lex.yy.c gmr.tab.* gmr.output gmr.xml gmr gmr.h tout





