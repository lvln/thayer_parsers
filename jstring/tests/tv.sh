#!/bin/bash
shopt -s nullglob
let n=1
for f in ../tests.src/pass.*
do
		cp $f ./pass.$n
		let n++;
done

shopt -s nullglob
let n=1
for f in ../tests.src/fail.*
do
		cp $f ./fail.$n
		let n++;
done


