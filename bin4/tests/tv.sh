#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "babb" > ./pass.1
echo -n "" > ./pass.2
echo -n "aaa" > ./pass.3
echo -n "bbababbaba" > ./pass.3
echo -n "bbababbaab" > ./pass.3

# [FAILING TESTS]
echo -n "a" > ./fail.1
echo -n "abc" > ./fail.2
echo -n "ababab" > ./fail.3

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
