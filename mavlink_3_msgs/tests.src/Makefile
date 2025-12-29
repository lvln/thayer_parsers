# Ben Cavanagh 10/19/25
# Makefile for generating MAVLink messages
# using the official headers library

# -Wno-address-of-packed-member to suppress a common
# "potential alignment" error from the library
CFLAGS=-Wall -pedantic -Wno-address-of-packed-member -std=c11 
GCC=gcc

MAV1=../../mavgen/c_library_v1
MAV2=../../mavgen/c_library_v2

all: mav_v1_tgen mav_v2_tgen

mav_v1_tgen: mav_tgen.c
	$(GCC) $(CFLAGS) -DMAKEFILE_SET_V1 -I$(MAV1) -o $@ $<

mav_v2_tgen: mav_tgen.c
	$(GCC) $(CFLAGS) -I$(MAV2) -o $@ $<

clean:
	rm -f *~ *.o *.txt *_tgen

