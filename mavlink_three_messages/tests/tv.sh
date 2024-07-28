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
SRCDIR="../../mavlink/mavlink_source_files"

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

# Array of message ids present in parser
msgIDs=(29 30 33)

# Loop through all message ids and generate tests
for id in "${msgIDs[@]}"; do
		# Generate tests for given id
		./tv ${id} > /dev/null

		echo "Tests for message ID ${id} generated"
done

# Create empty file for passing test
touch pass.10000

# Start counter for messages for individual drone flights.
let msgNum=1

if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null

		# Extract all messages of the given codes and save to temporary file
		./extractbymessageid ./run1.pcap ./temp.mav 29 30 33 >/dev/null

		# Count messages in temporary file
		./countmessages ./temp.mav > foo
		NUMMESS=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= NUMMESS; i++ )); do
				./extractbymessagenumber ./temp.mav pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done

		# Move all messages over to tests directory
		mv ./temp.mav ../../mavlink_three_messages/tests/pass.10001 > /dev/null

		# Clean out source file directory
		make clean > /dev/null
		popd
fi

if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make clean >/dev/null
		make > /dev/null

		# Extract all messages of the given codes and save to temporary file
		./extractbymessageid ./run2.pcap ./temp.mav 29 30 33 > /dev/null

		# Count the number of messages in temporary file
		./countmessages ./temp.mav > foo
		NUMMESS=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= NUMMESS; i++ )); do
				./extractbymessagenumber ./temp.mav pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done

		# Move all messages over to tests directory
		mv ./temp.mav ../../mavlink_three_messages/tests/pass.10002 > /dev/null

		# Clean out source directory
		make clean > /dev/null
		popd
fi

# More all of the tests to the tests directory
mv ${SRCDIR}/pass.* . > /dev/null

# Bring over an EVENT message and place it in a failing test file
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract EVENT message
		./extractbymessagenumber run1.pcap fail.10000 65 > /dev/null

		# Move message over to tests directory
		mv fail.10000 ../../mavlink_three_messages/tests/ > /dev/null

		# Clean source file directory
		make clean > /dev/null
		popd
fi

# All messages of the given types with a single invalid message type added at the end
cat pass.10001 fail.10000 > fail.10001
cat pass.10002 fail.10000 > fail.10002
