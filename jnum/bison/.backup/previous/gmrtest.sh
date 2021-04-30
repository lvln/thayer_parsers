#!/bin/bash
NUMPASS=6
NUMFAIL=3

echo "[Passing Tests]"
shopt -s nullglob
for (( t=1; t<=${NUMPASS}; t++ ))
do
		tst.sh "./gmr ../tests/pass.${t}"
done

echo "[Failing Tests]"
shopt -s nullglob
for (( t=1; t<=${NUMFAIL}; t++ ))
do
		tst.sh "./gmr ../tests/fail.${t}"
done
