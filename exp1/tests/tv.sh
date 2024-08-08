#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "sabc" > ./pass.1
echo -n "raac" > ./pass.2
echo -n "aaac" > ./pass.3
echo -n "sabcsabc" > ./pass.4
echo -n "" > pass.5
echo -n "accc" > ./pass.6

# [FAILING TESTS]
echo -n "abc" > ./fail.1
echo -n "acca" > ./fail.2
echo -n "acba" > ./fail.3
echo -n "aaacaac" > ./fail.4
echo -n "abcabc" > ./fail.5

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
