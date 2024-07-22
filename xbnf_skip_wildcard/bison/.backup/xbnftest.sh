#!/bin/bash
shopt -s nullglob
for f in ../tests/pass.*
do
		tester.sh pass "./xbnf $f"
done

shopt -s nullglob
for f in ../tests/fail.*
do
		tester.sh fail "./xbnf $f"
done
