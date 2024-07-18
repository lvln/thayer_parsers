#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "ab" > ./pass.1
echo -n "" > ./pass.2
echo -n "acac" > ./pass.3

# [FAILING TESTS]
echo -n "a" > ./fail.1
echo -n "ca" > ./fail.2
echo -n "aba" > ./fail.3

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
