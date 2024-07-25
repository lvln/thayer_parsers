CFLAGS=-Wall -pedantic -std=c11 -I../../utils -L../../lib
LIBS=-lutils -lcurl

all: tv

tv: tv.c
		gcc $(CFLAGS) tv.c $(LIBS) -o $@

clean:
				rm -f *~ tv *.o
				find . \( -name "pass.*" -o -name "fail.*" \) ! \( -name "pass.1000" -o -name "pass.1001" -o -name "pass.1002" -o -name "pass.1003" -o -name "fail.1000" -o -name "fail.1001" \) | xargs rm -f





