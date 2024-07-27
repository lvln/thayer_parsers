# MAVLink SCALED\_PRESSURE (repeated)
## Generates a parser for zero or more MAVLink SCALED_PRESSURE messages

NOTE: This differs from the parser in `mavlink_standalone_message` in that it accepts zero or multiple messages of type SCALED_PRESSURE.

All data packets were captured in `.pcap` files using Wireshark, a network protocol analyzer.

## Directory structure

* **xbnf** contains a working parser written in xbnf for a zero or more SCALED_PRESSURE MAVLink messages
* **bison** contains a working parser written in bison for zero or more SCALED_PRESSURE MAVLink messages
* **tests** contains a full set of passing and failing tests for the parser; it is recommended to clean this directory after testing since `tv.sh` generates roughtly 10,000 test files

## Data packet structure
MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding ground control stations.
The format of MAVLink messages is defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).
MAVLink 2.0 truncates empty (zero-filled bytes) at the end of the payload; it will never truncate the first byte of the payload regardless of if it is empty or not. The grammar is thus defined such that it allows for each payload to be any length.

**The message begins with a message-specific message header which is 10 bytes in length.**

* Byte 0: Magic value/version (always 0xFD for MAVLink 2.0)
* Byte 1: Payload length (0x10 for SCALED_PRESSURE)
* Byte 2: Incompatibility flag
* Byte 3: Compatibility flag
* Byte 4: Packet sequence
* Byte 5: System id
* Byte 6: Component id
* Bytes 7 - 9: Message id (0x00001D for SCALED_PRESSURE)

**SCALED_PRESSURE**: The pressure readings for the typical setup of one absolute and differential pressure sensor.

* Bytes 10 - 13: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`
* Bytes 14 - 17: *press_abs*: absolute pressure; *hPa*; `float`
* Bytes 18 - 21: *press_diff*: differential pressure; *hPa*; `float`
* Bytes 22 - 23: *temperature*: absolute pressure temperature; *cdegC*; `int16_t`
* Bytes 24 - 25: *temperature_press_diff*: differential pressure temperature (0, if not available). Report values of 0 (or 1) as 1 cdegC.; *cdegC*; `int16_t`

![SCALED_PRESSURE](./.images/SPImage.jpg)
  
All data packets are followed by a 2 byte message cyclic redundancy check which acts as a checksum (this will be bytes 26 - 27 in an untruncated SCALED_PRESSURE message).
