CFLAGS=-Wall -pedantic -std=c11 -I../../utils -L../../lib
LIBS=-lutils -lcurl

all: cleandata read extractdata untruncate countcodes

cleandata:
						gcc $(CFLAGS) cleandata.c $(LIBS) -o $@

read:
						gcc $(CFLAGS) read.c $(LIBS) -o $@

extractdata:
						gcc $(CFLAGS) extractdata.c $(LIBS) -o $@

untruncate:
						gcc $(CFLAGS) untruncate.c $(LIBS) -o $@

countcodes:
						gcc $(CFLAGS) countcodes.c $(LIBS) -o $@

clean:
				rm -f *~ cleandata read extractdata untruncate countcodes *.o





