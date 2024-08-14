#!/bin/bash

# Clean out the current directory.
make clean > /dev/null

# Copy the tests over from tests.src
cp -rf ../tests.src/* . &> /dev/null
