#!/bin/bash
rm -f ../tests/pass.* ../tests/fail.* ./pass.* ./fail.* 2> /dev/null > /dev/null

python tv.py

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
