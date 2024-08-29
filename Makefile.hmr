#
# THE STANDARD HAMMER MAKEFILE -- DO NOT CHANGE OR MOVE THIS FILE
# This file is used to clean, make, or run all Hammer parsers
#
CFLAGS= -Wall -pedantic -std=c11 \
				-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include\
				-I/usr/local/include -L/usr/local/lib\
				-I../../ppxml -L../../ppxml
LIBS= -lppxml -lhammer -lglib-2.0 
GCC=gcc

all:		
				$(GCC) $(CFLAGS) parser.c -o hmr $(LIBS)
				make --no-print-directory xml

xml:	
				./hmr -g

run:
				hmrtest.sh

clean:
				rm -f *~ *.o *.xml hmr hmr.output




