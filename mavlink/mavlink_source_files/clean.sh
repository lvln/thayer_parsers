#!/bin/bash

if [ $# -ne 2 ] ; then
		echo "usage: clean.sh inputFile outputFile"
		exit
fi

./cleandata $1 $2 > /dev/null 2> /dev/null

if [ $? -ne 0 ] ; then
		echo "usage: clean.sh inputFile outputFile"
		exit
fi
