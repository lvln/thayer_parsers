CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: cleandata extractbymessageid untruncate countcodes makemav extractbymessagenumber

cleandata: cleandata.c
						gcc $(CFLAGS) cleandata.c $(LIBS) -o $@

extractbymessageid: extractbymessageid.c
						gcc $(CFLAGS) extractbymessageid.c $(LIBS) -o $@

untruncate: untruncate.c
						gcc $(CFLAGS) untruncate.c $(LIBS) -o $@

countcodes: countcodes.c
						gcc $(CFLAGS) countcodes.c $(LIBS) -o $@

makemav: makemav.c
				gcc $(CFLAGS) makemav.c $(LIBS) -o $@

extractbymessagenumber: extractbymessagenumber.c
								 gcc $(CFLAGS) extractbymessagenumber.c $(LIBS) -o $@

clean:
				rm -f *~ cleandata extractbymessageid untruncate countcodes makemav extractbymessagenumber *.o
