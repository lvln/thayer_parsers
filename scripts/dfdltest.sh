#!/bin/bash
shopt -s nullglob

for f in ../tests/pass.*
do
    ../../scripts/tester.sh pass "./c/daffodil parse -V daffodil $f"
done

for f in ../tests/fail.*
do
    ../../scripts/tester.sh fail "./c/daffodil parse -V daffodil $f"
done
