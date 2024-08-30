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

# Make the vector libeary
pushd ../../utils/
make clean > /dev/null
make > /dev/null
popd

# Make the tools in the data processing library
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
let passSeed=($(ls ./pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
let failSeed=($(ls ./fail.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)

./tv 30 ${passSeed} ${failSeed} > /dev/null

# Find the values of pass and fail seed
let passSeed=($(ls ./pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
let failSeed=($(ls ./fail.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)

./tv 33 ${passSeed} ${failSeed} > /dev/null

# Find the values of pass and fail seed
let passSeed=($(ls ./pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
let failSeed=($(ls ./fail.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)

pushd ../../mavlink/mavlink_source_files/

# Build executables
make clean > /dev/null
make > /dev/null

# Extract a single message of each type from a drone flight
./extractonebyid rural.tlog ../../mavlink_3_msgs/tests.src/pass.${passSeed} 29
let passSeed++
./extractonebyid rural.tlog ../../mavlink_3_msgs/tests.src/pass.${passSeed} 30
let passSeed++
./extractonebyid rural.tlog ../../mavlink_3_msgs/tests.src/pass.${passSeed} 33
let passSeed++

# Extract all messages of each type from a flight
./extractbymessageid rural.tlog ../../mavlink_3_msgs/tests.src/pass.${passSeed} 29
let passSeed++
./extractbymessageid rural.tlog ../../mavlink_3_msgs/tests.src/pass.${passSeed} 30
let passSeed++
./extractbymessageid rural.tlog ../../mavlink_3_msgs/tests.src/pass.${passSeed} 33
let passSeed++

# All of the relevant messages from a single flight
./extractbymessageid rural.tlog ../../mavlink_3_msgs/tests.src/pass.${passSeed} 29 30 33
let passAll=passSeed
let passSeed++

# Empty file
touch pass.${passSeed}
let passSeed++

# Incorrect message type
./extractonebyid rural.tlog ../../mavlink_3_msgs/tests.src/fail.${failSeed} 31
let failOne=failSeed
let failSeed++

make clean > /dev/null

popd

cat pass.${passAll} fail.${failOne} > fail.${failSeed}
let failSeed++

# Clean the data processing library
pushd ../../mavlink/utils/
make clean > /dev/null
popd

# Clean the vector libeary
pushd ../../utils/
make clean > /dev/null
popd

pushd ../tests/

# Move tests to tests directory
make clean > /dev/null

# Import tests from tests.src
cp -rf ../tests.src/pass.* ../tests.src/fail.* . &> /dev/null

popd
