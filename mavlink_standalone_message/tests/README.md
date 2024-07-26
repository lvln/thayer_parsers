# Tests for MAVLink parser

Tests for the MAVLink parser which contains tests for all given byte values in all fields of all messages.

## Contents:

* `tv.c` auto-generates a range of tests, testing all ranges of byte values for all fields
* `pass.29.len` is a test for each possible payload length with random values assigned to wildcard fields and valid values assigned to enumerated fields 
* `pass.29.16.ind.val` is a passing test for SCALED_PRESSURE message at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.29.16.ind.val` is a failing test for a SCALED_PRESSURE message at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.29.16.header.field.val` is a failing test for a SCALED_PRESSURE message at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `pass.29.16.header.field.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `fail.29.16.extraByte` and `fail.29.16.oneByteLess` are failing tests for messages of the maximum length with one byte too many and one byte too few respectively
* `pass.29.16.crcx.val` are passing tests for messages of the maximum length with `crc` field `x` changed such that it contains a byte of value `val` 
* `pass.1000` and `pass.1001` are individual SCALED_PRESSURE messages from an actual flight with the drone
* `fail.1000` is an empty file
* `fail.1001` contains an EVENT message of the same length as a SCALED_PRESSURE message

## Usage:

* `make clean` to remove all old test files (except for tests from an actual drone flight) as well as executables
* `make` to build executable `tv` from `tv.c`
* `./tv.sh` generates roughtly 10,000 passing and failing tests for each message id present in the parser