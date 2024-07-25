#!/bin/bash

if [ $# -ne 1 ] ; then
		echo "usage: preprocess.sh inputFile"
		exit
fi

./removewrappers $1 > /dev/null 2> /dev/null

if [ $? -ne 0 ] ; then
		echo "usage: preprocess.sh inputFile"
		exit
fi
