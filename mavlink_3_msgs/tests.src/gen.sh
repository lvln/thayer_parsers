#!/bin/bash

# Clean test gen directory
make clean > /dev/null
make &> /dev/null

# Run message generators for each version & msg ID
# HEARTBEAT
./v1_mav_tgen 0
./v2_mav_tgen 0

# SYS_STATUS
./v1_mav_tgen 1
./v2_mav_tgen 1

# VFR_HUD
./v1_mav_tgen 74
./v2_mav_tgen 74

# Clear out old, move everything to test directory
rm ../tests/pass.* ../tests/fail.* &> /dev/null
# For-loop too because large numbers of files can
# exceed arg list max len
for f in ../tests/pass.* ../tests/fail.* ; do
    rm "$f" &> /dev/null
done

for f in ./pass.* ./fail.* ; do
    mv "$f" ../tests/ &> /dev/null
done
