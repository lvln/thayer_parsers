#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "a" > ./pass.1
echo -n "c" > ./pass.2
echo -n "ac" > ./pass.3
echo -n "ca" > ./pass.4
echo -n "ccaaacc" > ./pass.5
echo -n "aaaaa" > ./pass.6

# [FAILING TESTS]
echo -n "ab" > ./fail.1
echo -n "b" > ./fail.2
echo -n "cd" > ./fail.3
echo -n "aaaaaaaa" > ./fail.4
echo -n "dccc" > ./fail.5
echo -n "aaaaaa" > ./fail.6

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
