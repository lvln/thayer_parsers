# Tests for Space Packet Protocol parser

## Contents:

Assumes that ./tests is linked to somewhere to put the tests (e.g. ../tests).

* Test files `pass.1` - `pass.n` and `fail.1` - `fail.n` thoroughly test the parser
* `tv.sh` generates passsing and failing tests for the parser and deposits them in `../tests/`
* `tv.c` is used by `tv.sh` to generate failing tests for the parser
* `converttobyte.c` converts test files with bit field headers to byte field headers prior to depositing in `../tests/`

Other files:
* `bit2byte.[ch]` converts test files using a function
* `bit2byte` a program that reads a test then generates and prints the byte equivalent for
* `show.sh` compares the output of converttobyte to the output of bit2byte
