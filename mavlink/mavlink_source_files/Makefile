CFLAGS=-Wall -pedantic -std=c11 -I../../utils -L../../lib
LIBS=-lutils

all: cleandata extractbymessageid countcodes makemav extractbymessagenumber countmessages

cleandata: cleandata.c
						gcc $(CFLAGS) cleandata.c $(LIBS) -o $@

extractbymessageid: extractbymessageid.c
						gcc $(CFLAGS) extractbymessageid.c $(LIBS) -o $@

countcodes: countcodes.c
						gcc $(CFLAGS) countcodes.c $(LIBS) -o $@

makemav: makemav.c
				gcc $(CFLAGS) makemav.c $(LIBS) -o $@

extractbymessagenumber: extractbymessagenumber.c
								 gcc $(CFLAGS) extractbymessagenumber.c $(LIBS) -o $@

countmessages: countmessages.c
							gcc $(CFLAGS) countmessages.c $(LIBS) -o $@

clean:
				rm -f *~ cleandata extractbymessageid countcodes makemav extractbymessagenumber countmessages *.o
