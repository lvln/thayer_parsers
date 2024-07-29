#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

# Check number of arguments
if [ $# != 0 ]; then
		echo "usage: tv.sh"
		exit
fi

# This is the source directory path.
SRCDIR="../mavlink_source_files"

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

# Array of message IDs currently in the parser
msgIDs=(0 1 4 8 22 24 29 30 31 32 33 42 46 65 74 77 83 85 87 109 141 147 230 241 242 245 253 340 380 410 411 12901 12904)

# Loop through all message IDs
for id in "${msgIDs[@]}"; do
		# Generate the tests for the given message code
		./tv ${id}
		
		echo "Tests for message ID ${id} generated"
done

# Create a test file for full flights with the drone with the drone.
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null

		# Remove PCAP headers and extract only the MAVLink message portion
		./makemav ./run1.pcap ./pass.10000 > /dev/null

		# Move to tests directory
		mv ./pass.10000 ../tests > /dev/null

		make clean > /dev/null
		popd
fi

# Create a test file for full flights with the drone with the drone.
if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null

		# Remove PCAP headers and extract only the MAVLink message portion
		./makemav ./run2.pcap ./pass.10001 > /dev/null

		# Move to tests directory
		mv ./pass.10001 ../tests > /dev/null
		
		make clean > /dev/null
		popd
fi

# Start counter for messages for individual drone flights.
let msgNum=1

if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null

		# Count messages in run1 file
		./countmessages ./run1.pcap > foo
		numMess=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= numMess; i++ )); do
				./extractbymessagenumber ./run1.pcap pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done
		
		make clean > /dev/null
		popd
fi

if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make > /dev/null

		# Count messages in run2 file
		./countmessages ./run2.pcap > foo
		numMess=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= numMess; i++ )); do
				./extractbymessagenumber ./run2.pcap pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done
		
		make clean > /dev/null
		popd
fi

# More all of the tests to the tests directory
mv ${SRCDIR}/pass.* . > /dev/null

# Create an empty file
touch pass.10002 
