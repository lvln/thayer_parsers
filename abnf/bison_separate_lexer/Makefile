FLEX=flex
BISON=bison
CC=gcc
OFILES=abnf.tab.o abnf.o abnf_tb.o
CFLAGS=-Wall -pedantic -std=c11 -I.
BFLAGS=-d -t -v -x -Wno-other
EXEC=abnf

all: $(EXEC)

$(EXEC): $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o $@

abnf_tb.o: abnf_tb.c abnf.tab.h
	$(CC) $(CFLAGS) -c $< -o $@

abnf.tab.o: abnf.y
	$(BISON) $(BFLAGS) abnf.y -o abnf.tab.c
	$(CC) $(CFLAGS) -c abnf.tab.c -o $@

abnf.o: abnf.l abnf.tab.h
	$(FLEX) -o abnf.c abnf.l
	$(CC) $(CFLAGS) -c abnf.c -o $@

run:
	./abnftest.sh

clean:
	rm -rf *.o *.output *.tab.c *.tab.h abnf.c *.xml $(EXEC)
