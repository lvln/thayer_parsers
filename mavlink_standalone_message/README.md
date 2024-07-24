# MAVLink SCALED\_PRESSURE
## Generates a parser for a single MAVLink SCALED_PRESSURE message

NOTE: This differs from the parser in `mavlink_single_message_repeat` in that it accepts one and only one message of type SCALED_PRESSURE.

All data packets were captured in `.pcap` files using Wireshark, a network protocol analyzer.

## Directory structure

* **xbnf** contains a working parser written in xbnf for a single SCALED_PRESSURE MAVLink message
* **tests** contains a full set of passing and failing tests for the parser

## Data packet structure
MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding ground control stations.
The format of MAVLink messages is defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).

**The message begins with a message-specific message header which is 10 bytes in length.**

* Byte 0: Magic value/version (always 0xfd for MAVLink 2.0).
* Byte 1: Payload length (0x10 for SCALED_PRESSURE).
* Byte 2: Incompatibility flag.
* Byte 3: Compatibility flag.
* Byte 4: Packet sequence.
* Byte 5: System id.
* Byte 6: Component id.
* Bytes 7 - 9: Message id (0x00001D for SCALED_PRESSURE).

**SCALED_PRESSURE**: The pressure readings for the typical setup of one absolute and differential pressure sensor.

* Bytes 10 - 13: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`.
* Bytes 14 - 17: *press_abs*: absolute pressure; *hPa*; `float`.
* Bytes 18 - 21: *press_diff*: differential pressure; *hPa*; `float`.
* Bytes 22 - 23: *temperature*: absolute pressure temperature; *cdegC*; `int16_t`.
* Bytes 24 - 25: *temperature_press_diff*: differential pressure temperature (0, if not available). Report values of 0 (or 1) as 1 cdegC.; *cdegC*; `int16_t`.

![SCALED_PRESSURE](./.images/SPImage.jpg)
  
All data packets are followed by a 2 byte message cyclic redundancy check which acts as a checksum (this will be bytes 26 - 27 in a SCALED_PRESSURE message)..
