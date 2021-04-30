#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -n "a" > ./pass.1
echo -n "()" > ./pass.2
echo -n "(a)" > ./pass.3
echo -n "((a))" > ./pass.4
echo -n "(((((a)))))" > ./pass.5
echo -n "a,a" > ./pass.6
echo -n "a,a,a" > ./pass.7
echo -n "a,(a);()" > ./pass.8
echo -n "(a),(a)" > ./pass.9
echo -n "a,a,a;a,a,a" > ./pass.10
echo -n "a,a,(a,a,a,a;a,a,a,a;a,a,a,a);();(a,a;a)" > ./pass.11
echo -n "a,a,a,a;a,a,a,a;a,a,a,a" > ./pass.12
echo -n "(a,a,a)" > ./pass.13
echo -n "a,(a;a,a,a)" > ./pass.14
echo -n "a,a,a,a,()" > ./pass.15
echo -n "(a;a;a;a,a,a)" > ./pass.16
echo -n "a,(),(a);a,a" > ./pass.17
echo -n "a,a,a,(a;())" > ./pass.18
echo -n "a,a,a;a;a,a,a,a;a,a" > ./pass.19
echo -n "a;a;a;a,a,(a,a,a,a,a)" > ./pass.20
echo -n "(a,a,a,a;a,a,a;a,a;a)" > ./pass.21
echo -n "a,a,(a,a,a;a;a,a);a,a" > ./pass.22
echo -n "a,a,a,a,a;a,a,a,a,a" > ./pass.23
# [FAILING TESTS]
echo -n "aa" > ./fail.1
echo -n ")" > ./fail.2
echo -n "(aa)" > ./fail.3
echo -n "(((a))" > ./fail.4
echo -n "(a(a))" > ./fail.5
echo -n "aaaa" > ./fail.6
echo -n "" > ./fail.7
echo -n "," > ./fail.8
echo -n "a," > ./fail.9
echo -n "a,x" > ./fail.10
echo -n "a;();," > ./fail.11
echo -n "a,;" > ./fail.12
echo -n "ac" > ./fail.13
echo -n "(a,a,,a)" > ./fail.14
echo -n "(a,a,;a)" > ./fail.15
echo -n "a,a,a,aa" > ./fail.16
echo -n "a;a;a; a" > ./fail.17
echo -n "((a;a,a,))" > ./fail.18
echo -n "a,a,();;" > ./fail.19
echo -n "a;a;a;," > ./fail.20
echo -n "a,a,a;a,a,(a,a,a,a,a" > ./fail.21
echo -n "a;a,a,a;a,a,a;;a,a,a" > ./fail.22
echo -n "(a,a;a,a,a,a;a);a,a,a,,a" > ./fail.23
echo -n "a;a;a,a,a,a,aaa,a" > ./fail.24
echo -n "a,a,a,(a;a),ab,a,a;ac" > ./fail.25

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
