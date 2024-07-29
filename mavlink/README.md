# MAVLink

**Note: Best way to run tests is to use the `mavlinktest.sh` script inside of the xbnf directory. This allows for each message type to be tested, one at a time.**

This directory contains the complete definition for the MAVLink grammar. Please see `mavlink_standalone_message`, `mavlink_single_message-repeat` or `mavlink_three_messages` for smaller components of the MAVLink grammar.

All data packets were captured in `.pcap` files using Wireshark, a network protocol analyzer.

The usage of the `mavlinktest.sh` script is: `mavlinktest.sh [-v]` where `-v` indicates verbose output and the result of both passing and failing tests will be printed; non-verbose output only prints the results of failing tests.

## Directory structure

* **xbnf** contains a working parser written in xbnf for the MAVLink grammar
* **bison** contains a working parser written in bison the MAVLink grammar
* **tests** contains a full set of tests for the MAVLink parser
* **mavlink_source_files** contains preprocessors that format the data correctly as well as the PCAP files with data from drone flights
* **wireshark** contains the Wireshark plugin that dissects MAVLink messages as well as provides some level of statistical analysis on all MAVLink messages in a capture
* **utils** contains the MAVLink data processing library
* **lib** contains the compiled libraries

## Data packet structure

MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding ground control stations.
The format of MAVLink messages is defined [here](https://mavlink.io/en/guide/serialization.html) with the payload of each individual message defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).
MAVLink 2.0 truncates empty (zero-filled bytes) at the end of the payload; it will never truncate the first byte of the payload regardless of if it is empty or not. The grammar is thus defined such that it allows for each payload to be any length.

**Each message begins with a message-specific header which is 10 bytes in length.**

* Byte 0: Magic value/version (always 0xfd for MAVLink 2.0)
* Byte 1: Payload length
* Byte 2: Incompatibility flag
* Byte 3: Compatibility flag
* Byte 4: Packet sequence
* Byte 5: System id
* Byte 6: Component id
* Bytes 7 - 9: Message id (big endian)

**The is then followed by the message payload which is listed here for a few messages.**

* **GLOBAL_POSITION_INT**: The filtered global position (e.g. fused GPS and accelerometers). The position is in GPS-frame (right-handed, Z-up).
  * Bytes 10 - 13: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`
  * Bytes 14 - 17: *lat*: latitude; *degE7*; `int32_t`
  * Bytes 18 - 21: *lon*: longitude; *degE7*; `int32_t`
  * Bytes 22 - 25: *alt*; altitude; *mm*; `int32_t`
  * Bytes 26 - 29: *relative_alt*; altitude above ground; *mm*; `int32_t`
  * Bytes 30 - 31: *vx*; ground x speed (latitude, positive north); *cm/s*; `int16_t`
  * Bytes 32 - 33: *vy*; ground y speed (longitude, positive east); *cm/s*; `int16_t`
  * Bytes 34 - 35: *vz*; ground z speed (altitude, positive down); *cm/s*; `int16_t`
  * Bytes 36 - 37: *hdg*; vehicle heading (yaw angle); *cdeg*; `uint16_t`
  
  ![GLOBAL_POSITION_INT](./.images/GPIImage.jpg)
  
* **ATTITUDE**: The attitude in the aeronautical frame (right-handed, Z-down, Y-right, X-front, ZYX, intrinsic)
  * Bytes 10 - 13: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`
  * Bytes 14 - 17: *roll*; roll angle (-pi..+pi); *rad*; `float`
  * Bytes 18 - 21: *pitch*; pitch angle (-pi..+pi); *rad*; `float`
  * Bytes 22 - 25: *yaw*; yaw angle (-pi..+pi); *rad*; `float`
  * Bytes 26 - 29: *rollspeed*; roll angular speed; *rad/s*; `float`
  * Bytes 30 - 33: *pitchspeed*; pitch angular speed; *rad/s*; `float`
  * Bytes 34 - 37: *yawspeed*; yaw angular speed; *rad/s*; `float`
  
  ![ATTITUDE](./.images/AttitudeImage.jpg)
  
All data packets are followed by a 2 byte message cyclic redundancy check which acts as a checksum.

## Usage

In the `utils` directory:
* `make clean`
* `make`

If using `mavlinktest.sh` to test, in `xbnf` or `bison` directory:
* `make clean`
* `make`
* `mavlinktest.sh [-v]` to run full suite of tests described in `tests` directory

If building tests and testing manually (caution: this generates a large number of test files):
* In `tests` directory:
  * `make clean` to remove old tests and executables
  * `make` to build executables
  * `tv.sh` to generate tests
* In `xbnf` or `bison` directory:
  * `make clean`
  * `make`
  * `make run` to run all tests
