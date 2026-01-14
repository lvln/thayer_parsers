# Ben Cavanagh 10/19/25
# Makefile for generating MAVLink messages
# using the official headers library

# -Wno-address-of-packed-member to suppress a common
# "potential alignment" error from the library
# See https://mavlink.io/en/mavgen_c/#build-warnings
CFLAGS=-Wall -pedantic -Wno-address-of-packed-member -std=c11
GCC=gcc

MAV1=../../mavgen/c_library_v1
MAV2=../../mavgen/c_library_v2

all: v1_mav_tgen v2_mav_tgen

v1_mav_tgen: mav_tgen.c
	$(GCC) $(CFLAGS) -DMAKEFILE_SET_V1 -I$(MAV1) -o $@ $<

v2_mav_tgen: mav_tgen.c
	$(GCC) $(CFLAGS) -I$(MAV2) -o $@ $<

clean:
	rm -f *~ *.o *_tgen

