#!/bin/bash
echo "6 3" > ./tests/numtests

echo "[Success Tests]"
tester.sh PASS "./gmr ./tests/t1.input"
tester.sh PASS "./gmr ./tests/t2.input"
tester.sh PASS "./gmr ./tests/t3.input"
tester.sh PASS "./gmr ./tests/t4.input"
tester.sh PASS "./gmr ./tests/t5.input"
tester.sh PASS "./gmr ./tests/t6.input"

echo ""

echo "[Failure Tests]"
tester.sh FAIL "./gmr ./tests/t100.input"
tester.sh FAIL "./gmr ./tests/t101.input"
tester.sh FAIL "./gmr ./tests/t102.input"
