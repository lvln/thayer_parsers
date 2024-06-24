#!/bin/bash
rm -f ./pass.* ./fail.* 

make clean > /dev/null
make > /dev/null

./tv

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
