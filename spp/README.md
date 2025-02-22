# Space Packet Protocol

This directory contains a parsser for the packet primary headers of the Space Packet Protocol.

## Directory structure

* **xbnf** contains a working parser written in xbnf for the Space Packet Protocol headers
* **bison** contains a working parser written in bison for the Space Packet Protocol headers
* **tests** contains a full set of passing and failing tests for the parser (byte-based)
* **tests.src** installs test vectors in `tests` and contains headers with bit fields
* **utils** contains the fuzzer and bit to byte conversion source code
* **lib** contains the compiled libraries

## Header structure

The format of the headers for the Space Packet Protocol is defined in Section 4 of the [Space Packet Protocol Recommended Standard](https://public.ccsds.org/Pubs/133x0b2e2.pdf).

**The packet primary header has the following fields:**

* Bits 0 - 2: packet version number (always 000)
* Bits 3 - 15: pakcet identification field which is subdivided as follows:
  * Bit 3: packet type; 0 for a telemetry packet, 1 for a telecommand packet
  * Bit 4: secondary header flag; 1 if a packet secondary header is present, 0 if not
  * Bits 5 - 15: application process ID
* Bits 16 - 31: packet sequence control field which is subdivided as follows:
  * Bits 16 - 17: sequence flags; 00 if the packet contains a continuation segment of data, 01 if it contains the first segment of data, 10 if it contains the last segment of data , or 11 if it contains unsegmented data
  * Bits 18 - 31: packet sequence count or packet name
* Bits 32 - 47: packet data length
  
The packet primary header will then be followed by the packet data field wihich is 1 to 65536 octets in length and contains an optional packet secondary header field, followed by an optional data field.

## Usage

In the `../xbnf/bison/` directory:
* `make clean`
* `make`

Testing on the set of tests in `./tests/`:
* In `./xbnf/` or `./bison/` directory:
  * `make clean`
  * `make`
  * `make run` to run all tests
