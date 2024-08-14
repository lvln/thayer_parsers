#!/bin/bash

# Check arguments
if [ $# != 0 ]; then
		echo "usage: tv.sh"
		exit
fi

# Clean out directory and build executables
make clean > /dev/null
make > /dev/null

# Generate tests
./tv 29 1 1 > /dev/null

# Find the values of pass and fail seed
let passSeed=($(ls ./pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
let failSeed=($(ls ./fail.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)

./tv 30 ${passSeed} ${failSeed} > /dev/null

# Find the values of pass and fail seed
let passSeed=($(ls ./pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
let failSeed=($(ls ./fail.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)

./tv 33 ${passSeed} ${failSeed} > /dev/null
