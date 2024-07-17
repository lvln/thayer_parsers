#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "abc" > ./pass.1
echo -n "aac" > ./pass.2
echo -n "aac" > ./pass.3

# [FAILING TESTS]
echo -n "a" > ./fail.1
echo -n "ac" > ./fail.2
echo -n "aba" > ./fail.3
echo -n "" > ./fail.4

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
