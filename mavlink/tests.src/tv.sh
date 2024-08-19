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

# Clean out directory and build executables
make clean > /dev/null
make > /dev/null

# All message types currently in parser
msgIDs=(0 1 2 4 8 11 20 21 22 23 24 29 30 31 32 33 40 42 43 44 45 46 47 49 51 65 69 73 74 76 77 83 85 87 109 132 141 147 148 230 241 242 245 253 300 340 380 397 410 411 12901 12904)

# Move into source files directory
pushd ../mavlink_source_files/
make clean > /dev/null
make > /dev/null

# Check which IDs are in parser
uniqueIDs=($(./countids rural.tlog | awk '{print $NF}'))

popd

# Initial values for pass and fail seed
let passSeed=1

# Loop through all message IDs
for id in "${msgIDs[@]}"; do
		# Extract a single message from the drone flight if contained in there
		if [[ " ${uniqueIDs[@]} " =~ " ${id} " ]]; then
				# Move into source files directory directory for the purpose of generating test files
				pushd ../mavlink_source_files/
				
				# Clean the current source files out and make the executables
				make clean > /dev/null
				make > /dev/null

        ./extractonebyid rural.tlog pass.${passSeed} ${id} > /dev/null

				mv ./pass.* ../tests.src/ &> /dev/null
				
				make clean > /dev/null
				popd
		# Else generate a single test
    else
        ./tvsingle ${id} ${passSeed} > /dev/null
    fi

		# Find the values of pass and fail seed
		let passSeed=($(ls ./pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
done

# Make a MAVLink file from the full rural run and bring it over as a passing test.
pushd ../mavlink_source_files/
make clean > /dev/null
make > /dev/null
./makemav rural.tlog pass.${passSeed} > /dev/null
mv pass.${passSeed} ../tests.src &> /dev/null
make clean >/dev/null
popd

# Generate the failing tests
./tv > /dev/null
