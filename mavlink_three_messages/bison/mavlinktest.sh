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

pushd ../tests

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

# Array of message IDs currently in the parser
msgIDs=(29 30 33)

# Loop through all message IDs
for id in "${msgIDs[@]}"; do
		# Generate the tests for the given message code
		./tv ${id}
		
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
		CMD="./extractbymessageid ./run1.pcap ./temp.mav 29 30 33"
		{ ${CMD} >& /dev/null ; } >& /dev/null
		CMD="./countmessages ./temp.mav"
		
		# Save the number of messages to a temporary file
		${CMD} > foo
		numMess=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= numMess; i++ )); do
				CMD="./extractbymessagenumber ./temp.mav pass.${msgNum} ${i}"
				{ ${CMD} >& /dev/null ; } >& /dev/null
				let msgNum++
		done

		# Move all messages over to tests directory
		mv ./temp.mav ../../mavlink_three_messages/tests/pass.10001 > /dev/null
		
		make clean > /dev/null
		popd
fi

if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make clean >/dev/null
		make > /dev/null

		# Extract all messages of the given codes and save to temporary file
		CMD="./extractbymessageid ./run2.pcap ./temp.mav 29 30 33"
		{ ${CMD} >& /dev/null ; } >& /dev/null
		CMD="./countmessages ./temp.mav"
		
		# Save the number of messages to a temporary file
		${CMD} > foo
		numMess=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= numMess; i++ )); do
				CMD="./extractbymessagenumber ./temp.mav pass.${msgNum} ${i}"
				{ ${CMD} >& /dev/null ; } >& /dev/null
				let msgNum++
		done

		# Move all messages over to tests directory
		mv ./temp.mav ../../mavlink_three_messages/tests/pass.10002 > /dev/null
		
		make clean > /dev/null
		popd
fi

# More all of the tests to the tests directory
CMD="mv ${SRCDIR}/pass.* ."
{ ${CMD} >& /dev/null ; } >& /dev/null

# Bring over an EVENT message and place it in a failing test file
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract all SCALED_PRESURE messages and store them in a temporary file
		CMD="./extractbymessagenumber run1.pcap fail.10000 65"
		{ ${CMD} >& /dev/null ; } >& /dev/null

		# Move message over
		mv fail.10000 ../../mavlink_three_messages/tests/ > /dev/null

		make clean > /dev/null
		popd
fi

# All messages of the given types with a single invalid message type added at the end
cat pass.10001 fail.10000 > fail.10001
cat pass.10002 fail.10000 > fail.10002

popd

# Run the passing tests
shopt -s nullglob
for f in ../tests/pass.*; do
		# Run the parser on each passing test
		CMD="./gmr ${f}"
		{ ${CMD} >& /dev/null ; } >& /dev/null
		
		# Get the result from running the parser
		RESVAL=$?
		
		# Print only when tests fail if not in verbose mode
		if [ ${RESVAL} == 0 ]; then
				if [ $# == 1 ]; then
						echo -e "[PASS: ${CMD}]"
				fi
		else
				echo -e "===>>> [FAIL: ${CMD}]"
		fi
done

# Run the failing tests
shopt -s nullglob
for f in ../tests/fail.*; do
		# Run the parser on each passing test
		CMD="./gmr ${f}"
		{ ${CMD} >& /dev/null ; } >& /dev/null
		
		# Get the result from running the parser
		RESVAL=$?
		
		# Print only when tests fail if not in verbose mode
		if [ ${RESVAL} != 0 ]; then
				if [ $# == 1 ]; then
						echo -e "[PASS: ${CMD}]"
				fi
		else
				echo -e "===>>> [FAIL: ${CMD}]"
		fi
done

pushd ../tests/
make clean > /dev/null
popd
