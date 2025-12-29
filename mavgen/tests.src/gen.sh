#!/bin/bash

# Clean test gen directory
make clean > /dev/null
make &> /dev/null

# Run message generators for each version & type
# HEARTBEAT
./mav_v1_tgen 1 0 > /dev/null
#./mav_v2_tgen 2 0 > /dev/null

# SYS_STATUS
#./mav_v1_tgen 1 1 > /dev/null
#./mav_v2_tgen 2 1 > /dev/null

# VFR_HUD
#./mav_v1_tgen 1 74 > /dev/null
#./mav_v2_tgen 2 74 > /dev/null

# Clear out old, move everything to test directory
rm ../tests/pass.* ../tests/fail.* &> /dev/null
mv ./pass.* ./fail.* ../tests/ &> /dev/null

