#!/bin/bash
shopt -s nullglob
for f in ../tests/pass.*
do
		tester.sh pass "./gmr $f"
done

shopt -s nullglob
for f in ../tests/fail.*
do
		tester.sh fail "./gmr $f"
done
