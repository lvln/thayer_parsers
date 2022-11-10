#!/bin/bash
if [ $# != 0 ] ; then
		echo "usage: backup.sh"
		exit -1
fi
if [ ! -d "./.backup" ] ; then
		mkdir ./.backup
		echo "[created .backup]"
fi
if [ ! -d "./.backup/previous" ] ; then
		mkdir ./.backup/previous
		echo "[created .backup/previous]"
fi
# save the previous backup
shopt -s nullglob
for f in ./.backup/*
do
		if [ -f $f ] ; then
				mv -f $f ./.backup/previous
		fi
done
# copy everything to .backup
shopt -s nullglob
for f in *.[ch] *.[ly] *.sh *.xbnf *.xml *.cpp *.hpp Makefile README README.md
do
		if [ -f $f ] ; then
				echo "[saving $f]"
				cp -f $f ./.backup
		fi
done
