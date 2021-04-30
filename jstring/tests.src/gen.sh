#!/bin/bash
rm -f pass.* fail.* numtests

# [PASSING TESTS]
echo -n "\"\""> ./pass.jstr.1
echo -n "\"abd\""> ./pass.jstr.2
echo -n "\"129\""> ./pass.jstr.3
echo -n "\"abcdefghijklmnopqrstuvwzyz\"" > ./pass.jstr.4
echo -n "\"ABCDEFGHIJKLMNOPQRSTUVWXYZ\"" > ./pass.jstr.5
echo -n "\"0123456789\""> ./pass.jstr.6
echo -n "\"\b\"" > ./pass.jstr.7
echo -n "\"\f\"" > ./pass.jstr.8
echo -n "\"\n\"" > ./pass.jstr.9
echo -n "\"\r\"" > ./pass.jstr.10
echo -n "\"\t\"" > ./pass.jstr.11
echo -n "\"\\\"\"" > ./pass.jstr.12
echo -n "\"\\\\\"" > ./pass.jstr.13
echo -n "\"\\/\"" > ./pass.jstr.14
echo -n "\"\u0129\"" > ./pass.jstr.15
echo -n "\"\uaFAf\"" > ./pass.jstr.16
echo -n "\"aaa\ua12fxxx\"" > ./pass.jstr.17
echo -n "\"'\"" > ./pass.jstr.18
echo -n "\" !#$%&'()*+,-./:;<=>?@[]^_\`{|}~\""> ./pass.jstr.19
echo -n "\"a'aaa!@#$%^&*()_-{}[]|=+;:\\\\<>,.?\\\"x\"" > ./pass.jstr.20

# [FAILING TESTS]
echo -n "\"\\\"" > ./fail.jstr.1
echo -n "\"aaa\xaaa\"" > ./fail.jstr.2
echo -n "\"aaa\paaa\"" > ./fail.jstr.3
echo -n "\"aaa\uxaaa\"" > ./fail.jstr.4
echo -n "\"aaa\aaa\"" > ./fail.jstr.5
echo -n "\"aaa\"aaa\"" > ./fail.jstr.6
echo -n "\"aaa\uabaxaaa\"" > ./fail.jstr.7

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests


