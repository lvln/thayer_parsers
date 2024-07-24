# Tests for MAVLink parser

Tests for the MAVLink parser which contains tests for all given byte values in all fields of all messages.

## Contents:

* `tv.c` auto-generates a range of tests, testing all ranges of byte values for all fields
* `pass.msgID.len` is a test for each possible payload length with random values assigned to wildcard fields and valid values assigned to enumerated fields 
* `pass.msgID.len.ind.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.msgID.len.ind.val` is a failing test for a given mesage ID at its maximum payload length (all fields present) where `ind` is the index of the payload field being tested and `val` is the byte value being tested
* `fail.msgID.len.header.field.val` is a failing test for a given mesage ID at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `pass.msgID.len.header.field.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) in the header field where `field` is the name of the header field being tested and val is the byte value being tested
* `fail.msgID.len.extraByte` and `fail.msgID.len.oneByteLess` are failing tests for messages of the maximum length with one byte too many and one byte too few respectively
* `pass.msgID.len.crc[x].val` are passing tests for messages of the maximum length with `crc` field `x` changed such that it contains a byte of value `val` 

## Usage:

* `make clean` to remove all old test files as well as executables
* `make` too build executable `tv` from `tv.c`
* `./tv.sh` generates passing and failing tests for each message id present in the parser
