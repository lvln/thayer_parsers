## Generates a parser for zero or more MAVLink SCALED_PRESSURE messages

All data packets were captured in `.pcap` files using Wireshark, a network protocol analyzer.

## Directory structure

* **xbnf** contains a working parser written in xbnf for a zero or more SCALED_PRESSURE MAVLink messages
* **bison** contains a working parser written in bison for zero or more SCALED_PRESSURE MAVLink messages
* **tests** contains a full set of passing and failing tests for the parser
* **tests.src** generates test vectors and installs them in `tests`

## Data packet structure
MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding ground control stations.
The format of MAVLink messages is defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).
Accoring to the [MAVLink serialization documentation](https://mavlink.io/en/guide/serialization.html), MAVLink 2.0 truncates empty (zero-filled bytes) at the end of the payload; it will never truncate the first byte of the payload regardless of if it is empty or not. The grammar is thus defined such that it allows for each payload to be any length.


**A MAVLink 1.0 message begins with a message-specific message header which is 6 bytes in length.**

* Byte 0: Magic value/version (0xFE for MAVLink 1.0)
* Byte 1: Payload length (0x10 for SCALED_PRESSURE)
* Byte 2: Packet sequence
* Byte 3: System id
* Byte 4: Component id
* Byte 5: Message id (0x1D for SCALED_PRESSURE)

**A MAVLink 2.0 message also begins with a message-specific message header which is 10 bytes in length.**

* Byte 0: Magic value/version (0xFD for MAVLink 2.0)
* Byte 1: Payload length (0x10 for SCALED_PRESSURE)
* Byte 2: Incompatibility flag (0x01 indicates a signed message)
* Byte 3: Compatibility flag
* Byte 4: Packet sequence
* Byte 5: System id
* Byte 6: Component id
* Bytes 7 - 9: Message id (0x00001D for SCALED_PRESSURE)


**SCALED_PRESSURE**: The pressure readings for the typical setup of one absolute and differential pressure sensor.

* Bytes 0 - 3: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`
* Bytes 4 - 7: *press_abs*: absolute pressure; *hPa*; `float`
* Bytes 8 - 11: *press_diff*: differential pressure; *hPa*; `float`
* Bytes 12 - 13: *temperature*: absolute pressure temperature; *cdegC*; `int16_t`
* Bytes 14 - 15: *temperature_press_diff*: differential pressure temperature (0, if not available). Report values of 0 (or 1) as 1 cdegC.; *cdegC*; `int16_t`

![SCALED_PRESSURE](./.images/SPImage.jpg)
  
All data packets are followed by a 2 byte message cyclic redundancy check which acts as a checksum (this will be bytes 26 - 27 in an untruncated SCALED_PRESSURE message).
If a MAVLink 2.0 message is signed (incompatibility flag 0x01), it will also contain a 13 byte signature at the end.

## Usage

In the `../xbnf/bison/` directory:
* `make clean`
* `make`

Fuzz test the parser using `mavlinktest.sh` in `./xbnf/` or `./bison/`:
* In `./xbnf/` or `./bison/` directory:
  * `make clean`
  * `make`
  * `mavlinktest.sh [-v]` to fuzz test the parser

If testing on the more limited set of tests in `./tests/`:
* In `./xbnf/` or `./bison/` directory:
  * `make clean`
  * `make`
  * `make run` to run all tests
