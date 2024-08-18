#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "a" > ./pass.1
echo -n "()" > ./pass.2
echo -n "(a)" > ./pass.3
echo -n "(())" > ./pass.4
echo -n "((a))" > ./pass.5
echo -n "(((((a)))))" > ./pass.6

# [FAILING TESTS]
echo -n "aa" > ./fail.1
echo -n "ac" > ./fail.2
echo -n ")" > ./fail.3
echo -n "((())" > ./fail.4
echo -n "((())))" > ./fail.5

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
