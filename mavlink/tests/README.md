# Tests for MAVLink parser

Tests for the MAVLink parser which contains tests for all given byte values in all fields of all messages.

Note: If testing all fields, it is best to test this parser using the `mavlinktest.sh` script in the `xbnf`/`bison` directories due to the large number of test files produced when running `tv.sh`.

## Contents:

* `pass.1` and `pass.2` are passing tests for two complete flights with the drone (contain multiple message types)
* `pass.3` - `pass.n` are passing tests for each individual message for two complete flights flights with the drone
* `tv.c` auto-generates a range of tests, testing all ranges of byte values for all fields
* `tv.sh [-l]` creates all of the above- and below-described test files using `tv.c` if using the `-l` flag; generates only the above-described tests if not using the '-l' flag
* `pass.msgID.len` is a test for each possible payload length with random values assigned to wildcard fields and valid values assigned to enumerated fields 
* `pass.msgID.len.ind.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.msgID.len.ind.val` is a failing test for a given mesage ID at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.msgID.len.header.field.val` is a failing test for a given mesage ID at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `pass.msgID.len.header.field.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `fail.msgID.len.extraByte` and `fail.msgID.len.oneByteLess` are failing tests for messages of the maximum length with one byte too many and one byte too few respectively
* `pass.msgID.len.crcx.val` are passing tests for messages of the maximum length with `crc` field `x` changed such that it contains a byte of value `val` 

## Usage:

In the `../../mavlink/utils` directory:
* `make clean`
* `make`

Back in the `tests` directory:
* `make clean`
* `make`
* `./tv.sh -l` to generate all test cases
