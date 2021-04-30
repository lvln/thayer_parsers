#!/bin/bash
echo "[Passing Tests]"
shopt -s nullglob
for f in ../tests/pass.*
do
		tst.sh "./gmr $f"
done

echo "[Failing Tests]"
shopt -s nullglob
for f in ../tests/fail.*
do
		tst.sh "./gmr $f"
done
