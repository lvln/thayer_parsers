CFLAGS=-Wall -pedantic -std=c11

all: read cleandata extractdata countcodes echo

read.o: read.c
				gcc $(CFLAGS) -c read.c

read: read.o
		  gcc $(CFLAGS) read.o -o read

cleandata.o: cleandata.c
				gcc $(CFLAGS) -c cleandata.c

cleandata: cleandata.o
		  gcc $(CFLAGS) cleandata.o -lm -o cleandata

extractdata.o: extractdata.c
				gcc $(CFLAGS) -c extractdata.c

extractdata: extractdata.o
		  gcc $(CFLAGS) extractdata.o -lm -o extractdata

countcodes.o: countcodes.c
				gcc $(CFLAGS) -c countcodes.c

countcodes: countcodes.o
		  gcc $(CFLAGS) countcodes.o -o countcodes

echo.o: echo.c
				gcc $(CFLAGS) -c echo.c

echo: echo.o
		  gcc $(CFLAGS) echo.o -o echo

clean:
			rm -f *.o read cleandata extractdata countcodes echo
