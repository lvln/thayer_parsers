#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

# Check number of arguments
if [ $# != 0 ] && [ $# != 1 ] ; then
		echo "usage: tv.sh [-l]"
		exit
fi

# Check that second argument is -l
if [ $# == '1' ]; then
		if [ $1 != '-l']; then
				echo "usage: tv.sh [-l]"
				exit
		fi
fi

# This is the source directory path.
SRCDIR="../../mavlink/mavlink_source_files"

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

if [ $# == 1 ]; then
		# Array of message ids present in parser
		msgIDs=(29 30 33)
		
		# Loop through all message ids and generate tests
		for id in "${msgIDs[@]}"; do
				# Generate tests for given id
				./tv ${id} > /dev/null
				
				echo "Tests for message ID ${id} generated"
		done
fi

# Start counter for messages for individual drone flights.
let msgNum=3

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
		mv ./temp.mav ../../mavlink_three_messages/tests/pass.1 > /dev/null

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
		mv ./temp.mav ../../mavlink_three_messages/tests/pass.2 > /dev/null

		# Clean out source directory
		make clean > /dev/null
		popd
fi

# Move all of the tests to the tests directory
mv ${SRCDIR}/pass.* . > /dev/null

# Bring over an EVENT message and place it in a failing test file
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract EVENT message
		./extractbymessagenumber run1.pcap fail.1 65 > /dev/null

		# Move message over to tests directory
		mv fail.1 ../../mavlink_three_messages/tests/ > /dev/null

		# Clean source file directory
		make clean > /dev/null
		popd
fi

# All messages of the given types with a single invalid message type added at the end
cat pass.1 fail.1 > fail.2
cat pass.2 fail.1 > fail.3
