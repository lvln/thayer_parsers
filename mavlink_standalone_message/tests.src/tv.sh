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
