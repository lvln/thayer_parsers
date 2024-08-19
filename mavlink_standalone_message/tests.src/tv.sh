#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

# Check arguments
if [ $# != 0 ]; then
		echo "usage: tv.sh"
		exit
fi

# Clean out and build library.
pushd ../../mavlink/utils/
make clean > /dev/null
make > /dev/null
popd

# Clean out directory and build executables
make clean > /dev/null
make > /dev/null

# Generate tests
./tv 29 1 1 > /dev/null

# Clean out library.
pushd ../../mavlink/utils/
make clean > /dev/null
popd
