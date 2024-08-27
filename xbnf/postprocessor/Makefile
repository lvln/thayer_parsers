CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils
GCC=gcc

all: postprocessor

postprocessor: postprocessor.c
				 			 $(GCC) $(CFLAGS) postprocessor.c $(LIBS) -o $@

clean:
			rm -f *~ postprocessor *.o
