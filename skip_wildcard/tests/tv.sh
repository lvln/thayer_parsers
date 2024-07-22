#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "abc" > ./pass.1
echo -n "aac" > ./pass.2
echo -n "aac" > ./pass.3

# [FAILING TESTS]
echo -n "abbc" > ./fail.1
echo -n "accc" > ./fail.2
echo -n "ac" > ./fail.3
echo -n "abd" > ./fail.4
echo -n "cbc" > ./fail.5
echo -n "" > ./fail.6

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
