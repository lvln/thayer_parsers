#!/bin/bash
NUMPASS=6
NUMFAIL=5

echo "[Passing Tests]"
shopt -s nullglob
for f in ../tests/pass.*
do
		tst.sh "./gmr $f -d &> $f.trace"
done

echo "[Failing Tests]"
shopt -s nullglob
for f in ../tests/fail.*
do
		tst.sh "./gmr $f -d &> $f.trace"
done
