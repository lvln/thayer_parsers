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

# Find the values of pass and fail seed
let passSeed=($(ls ../tests/pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
let failSeed=($(ls ../tests/fail.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)

pushd ../../mavlink/mavlink_source_files/
make clean > /dev/null
make > /dev/null

# Bring in a message from an actual drone flight
./extractonebyid rural.tlog ../../mavlink_standalone_message/tests.src/pass.${passSeed} 29
let passOne=passSeed
let passSeed++

# Bring in all the messages from the flight
./extractbymessageid rural.tlog ../../mavlink_standalone_message/tests.src/fail.${failSeed} 29
let failAll=failSeed
let failSeed++

# Create a test with the incorrect message type
./extractonebyid rural.tlog ../../mavlink_standalone_message/tests.src/fail.${failSeed} 30
let failOne=failSeed
let failSeed++

make clean > /dev/null

popd

# Create further failing test with incorrect message types appended to correct message type passes
cat pass.${passOne} fail.${failOne} > fail.${failSeed}
let failSeed++

# Create empty file
touch fail.${failSeed}
let failSeed++

# Clean out library.
pushd ../../mavlink/utils/
make clean > /dev/null
popd
