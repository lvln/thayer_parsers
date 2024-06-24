#!/bin/bash
rm -f ./pass.* ./fail.* 

make clean > /dev/null
make > /dev/null

cp ../mavlink_source_files/pass.* .
cp ../mavlink_source_files/fail.* .

./tv

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
