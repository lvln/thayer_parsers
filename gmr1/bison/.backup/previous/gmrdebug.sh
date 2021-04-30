#!/bin/bash
NUMPASS=23
NUMFAIL=25

echo "[Passing Tests]"
shopt -s nullglob
for (( t=1; t<=${NUMPASS}; t++ ))
do
		tst.sh "./gmr ../tests/pass.${t} -d &> ../tests/pass.${t}.trace"
done

echo "[Failing Tests]"
shopt -s nullglob
for (( t=1; t<=${NUMFAIL}; t++ ))
do
		tst.sh "./gmr ../tests/fail.${t} -d &> ../tests/fail.${t}.trace"
done
