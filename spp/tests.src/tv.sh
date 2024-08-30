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

# Make and clean vector
pushd ../../utils/
make clean > /dev/null
make > /dev/null
popd

# Make and clean fuzzer
pushd ../utils/
make clean > /dev/null
make > /dev/null
popd

# Clean test directory
pushd ../tests/
make clean > /dev/null
popd

# Clean out directory and build executables
make clean > /dev/null
make > /dev/null

# Generate tests
./tv 1 1

# Convert tewsts to byte-based tests and deposit in tests directory
for f in pass.*; do
		./converttobyte ${f} ../tests/${f}
done

for f in fail.*; do
		./converttobyte ${f} ../tests/${f}
done

# Clean out utils
pushd ../utils/
make clean > /dev/null
popd

# Clean vector
pushd ../../utils/
make clean > /dev/null
popd

