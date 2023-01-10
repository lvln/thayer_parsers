#!/bin/bash
shopt -s nullglob
for f in ../tests/pass.*
do
		tester.sh pass "./abnf $f"
done

shopt -s nullglob
for f in ../tests/fail.*
do
		tester.sh fail "./abnf $f"
done
