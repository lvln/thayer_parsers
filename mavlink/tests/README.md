# Tests for MAVLink parser

Tests for the MAVLink parser which contains tests for all given byte values in all fields of all messages.

## Contents:

* `tv.c` auto-generates a range of tests, testing all ranges of byte values for all fields
* `pass.msgID.len` is a test for each possible payload length with random values assigned to wildcard fields and valid values assigned to enumerated fields 
* `pass.msgID.len.ind.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) where `ind` is the index of field being tested and `val` is the byte value being tested
* `fail.msgID.len.ind.val` is a passing test for a given mesage ID at its maximum payload length (all fields present) where `ind` is the index of field being tested and `val` is the byte value being tested
* `fail.xxx.yyy` is an original message with message id xxx from a drone flight with a specific constant field changed:
  * yyy = 0: MAVLink code is changed
  * yyy = 1: Kmessage id is changed
  * yyy = 2: payload length value is changed
  * yyy = 3: one to few bytes is written
  * yyy = 4: one to many bytes is written
  * yyy = 5: message family value is changed
  * yyy = 6: ip version/header length field value is changed
  * yyy = 7: time to live value is changed
  * yyy = 8: source port value is changed
  * yyy = 9: destination port value is changed

## Usage:

* `make clean` to remove all old test files as well as executables
* `make` too build executable `tv` from `tv.c`
* `./tv` generates passing and failing tests based on the contents of `inputFile`
