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
pushd ../../xbnf/utils/
make clean > /dev/null
make > /dev/null
popd

# Make and clean utils
pushd ../utils/
make clean > /dev/null
make > /dev/null
popd

# Clean out directory and build executables
make clean > /dev/null
make > /dev/null

# Generate tests
./tv 1 1

# Clean out utils
pushd ../utils/
make clean > /dev/null
popd

# Clean vector
pushd ../../xbnf/utils/
make clean > /dev/null
popd

pushd ../tests/

# Move tests to tests directory
make clean > /dev/null

# Import tests from tests.src
cp -rf ../tests.src/pass.* ../tests.src/fail.* . &> /dev/null

popd
