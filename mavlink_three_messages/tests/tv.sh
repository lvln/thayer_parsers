#!/bin/bash

# Clean out the directory.
make clean > /dev/null

# Import tests from tests.src
cp -rf ../tests.src/* . &> /dev/null
