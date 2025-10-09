#!/bin/bash
shopt -s nullglob
for f in ../tests/pass.*
do
		tester.sh pass "daedalus run spec.ply -i $f"
done

shopt -s nullglob
for f in ../tests/fail.*
do
		tester.sh fail "daedalus run spec.ply -i $f"
done
