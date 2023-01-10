#!/bin/bash
rm pass.* fail.* numtests

# [PASSING TESTS]
echo -n "1" > ./pass.jnum.1
echo -n "0" > ./pass.jnum.2
echo -n "10" > ./pass.jnum.3
echo -n "11" > ./pass.jnum.4
echo -n "100" > ./pass.jnum.5
echo -n "101" > ./pass.jnum.6
echo -n "110" > ./pass.jnum.7
echo -n "111" > ./pass.jnum.8
echo -n "1010101010" > ./pass.jnum.9
echo -n "2" > ./pass.jnum.10
echo -n "3" > ./pass.jnum.11
echo -n "20" > ./pass.jnum.12
echo -n "30" > ./pass.jnum.13
echo -n "200" > ./pass.jnum.14
echo -n "300" > ./pass.jnum.15
echo -n "12" > ./pass.jnum.16
echo -n "123" > ./pass.jnum.17
echo -n "120" > ./pass.jnum.18
echo -n "230" > ./pass.jnum.19
echo -n "310" > ./pass.jnum.20
echo -n "1230" > ./pass.jnum.21
echo -n "12301" > ./pass.jnum.22
echo -n "123012" > ./pass.jnum.23
echo -n "1230123" > ./pass.jnum.24
echo -n "123123123123" > ./pass.jnum.25
echo -n "1234567890" > ./pass.jnum.26
echo -n "-0.5" > ./pass.jnum.27
echo -n "0.5" > ./pass.jnum.28
echo -n "-129" > ./pass.jnum.29
echo -n "129.525" > ./pass.jnum.30
echo -n "592.076E-555" > ./pass.jnum.31
echo -n "592.176e+555" > ./pass.jnum.32
echo -n "592.176e555" > ./pass.jnum.33

# [FAILING TESTS]
echo -n "a" > ./fail.jnum.1
echo -n "-a" > ./fail.jnum.2
echo -n "-02.0" > ./fail.jnum.3
echo -n "592.176e*555" > ./fail.jnum.4
echo -n "592.176e5.55" > ./fail.jnum.5
echo -n "592
176" > ./fail.jnum.6
echo -n "592\176" > ./fail.jnum.7
echo -n "592'176" > ./fail.jnum.8
echo -n "02" > ./fail.jnum.9

# generate a file with the number of pass and fail tests
NUMPASS=(pass.*)
NUMFAIL=(fail.*)
echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests


