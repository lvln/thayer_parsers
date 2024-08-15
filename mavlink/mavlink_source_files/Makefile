CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils

all: extractbymessageid countids makemav makepcap extractbymessagenumber extractonebyid

extractbymessageid: extractbymessageid.c
						gcc $(CFLAGS) extractbymessageid.c $(LIBS) -o $@

extractonebyid: extractonebyid.c
								gcc $(CFLAGS) extractonebyid.c $(LIBS) -o $@

countids: countids.c
				 	gcc $(CFLAGS) countids.c $(LIBS) -o $@

makemav: makemav.c
				 gcc $(CFLAGS) makemav.c $(LIBS) -o $@

makepcap: makepcap.c
					gcc $(CFLAGS) makepcap.c $(LIBS) -o $@

extractbymessagenumber: extractbymessagenumber.c
								 gcc $(CFLAGS) extractbymessagenumber.c $(LIBS) -o $@

clean:
				rm -f *~ countids extractbymessageid countcodes makemav extractbymessagenumber makepcap extractonebyid *.o
