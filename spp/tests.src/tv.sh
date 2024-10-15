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

# verify a destination exists for the data
if [ ! -d ./tests ]; then
		echo "./tests does not exist (link to destination)"
		exit
fi

# clean and build
make clean
make

# Generate tests
./tv 1 1

# Convert tests to byte-based tests and deposit in ./tests directory
# NOTE: ./tests should be a link to the destination location (e.g. ../tests)
for f in pass.*; do
		./converttobyte ${f} ./tests/${f}
done

for f in fail.*; do
		./converttobyte ${f} ./tests/${f}
done

