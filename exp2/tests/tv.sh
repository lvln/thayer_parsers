#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "saebc" > ./pass.1
echo -n "rawac" > ./pass.2
echo -n "aarac" > ./pass.3
echo -n "saabcsaabc" > ./pass.4
echo -n "" > pass.5
echo -n "acrcc" > ./pass.6

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
