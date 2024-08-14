#!/bin/bash

# Clean out directory
make clean > /dev/null

# Import tests from tests.src
cp -rf ../tests.src/pass.* ../tests.src/fail.* . &> /dev/null
