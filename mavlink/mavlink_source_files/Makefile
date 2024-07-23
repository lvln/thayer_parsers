CFLAGS=-Wall -pedantic -std=c11 -I../../utils -L../../lib
LIBS=-lutils -lcurl

all: cleandata extractdata untruncate countcodes removewrappers

cleandata: cleandata.c
						gcc $(CFLAGS) cleandata.c $(LIBS) -o $@

extractdata: extractdata.c
						gcc $(CFLAGS) extractdata.c $(LIBS) -o $@

untruncate: untruncate.c
						gcc $(CFLAGS) untruncate.c $(LIBS) -o $@

countcodes: countcodes.c
						gcc $(CFLAGS) countcodes.c $(LIBS) -o $@

removewrappers: removewrappers.c
						gcc $(CFLAGS) removewrappers.c $(LIBS) -o $@

clean:
				rm -f *~ cleandata extractdata untruncate countcodes removewrappers *.o
