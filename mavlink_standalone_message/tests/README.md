# Tests for MAVLink parser

Tests for the MAVLink parser which contains tests for all given byte values in all fields of all messages.

Note: It is best to test this parser using the `mavlinktest.sh` script in the `xbnf`/`bison` directories due to the large number of test files produced when running `tv.sh`.

## Contents:

* `tv.c` auto-generates a range of tests, testing all ranges of byte values for all fields
* `tv.sh` creates all of the below-described test files using `tv.c`
* `pass.29.len` is a test for each possible payload length with random values assigned to wildcard fields and valid values assigned to enumerated fields 
* `pass.29.16.ind.val` is a passing test for SCALED_PRESSURE message at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.29.16.ind.val` is a failing test for a SCALED_PRESSURE message at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.29.16.header.field.val` is a failing test for a SCALED_PRESSURE message at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `pass.29.16.header.field.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `fail.29.16.extraByte` and `fail.29.16.oneByteLess` are failing tests for messages of the maximum length with one byte too many and one byte too few respectively
* `pass.29.16.crcx.val` are passing tests for messages of the maximum length with `crc` field `x` changed such that it contains a byte of value `val` 
* `pass.1` - `pass.n` are all individual SCALED_PRESSURE messages from an actual flight with the drone
* `fail.1` is an empty file
* `fail.2` contains an EVENT message (of the same length as a SCALED_PRESSURE message)

## Usage:

In the `../../mavlink/utils` directory:
* `make clean`
* `make`

Back in the `tests` directory:
* `make clean`
* `make`
* `./tv.sh` generates roughly 10,000 passing and failing tests for each message ID present in the parser
