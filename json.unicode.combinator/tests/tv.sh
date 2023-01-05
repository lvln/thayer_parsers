#!/bin/bash
shopt -s nullglob
let n=1
for f in ../../json/tests.src/pass.* ../tests.unicode/pass.*
do
		cp $f ./pass.$n
		let n++;
done

shopt -s nullglob
let n=1
for f in ../../json/tests.src/fail.* ../tests.unicode/fail.*
do
		cp $f ./fail.$n
		let n++;
done


