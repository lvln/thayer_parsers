# Tests for MAVLink parser

## Contents:

* `tv.c` autop-generates a range of tests
* `pass.xxx` is an original message from a drone flight, where xxx is the message id
* `pass.xxx.yyy` is an original message with message id xxx from a drone flight with non-constant field number yyy changed
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
* `make run inputFile` generates passing and failing tests based on the contents of `inputFile`
