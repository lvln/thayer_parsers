# MAVLink 10 messages

This directory contains the grammar for 10 MAVLink message types.

## Directory structure

* **xbnf** contains a working parser written in xbnf for the MAVLink grammar
* **bison** contains a working parser written in bison the MAVLink grammar
* **tests** contains a full set of passing and failing tests for the parser
* **tests.src** installs test vectors into `tests`

## Data packet structure

MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding ground control stations.
The format of MAVLink messages is defined [here](https://mavlink.io/en/guide/serialization.html) with the payload of each individual message defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).
Accoring to the [MAVLink serialization documentation](https://mavlink.io/en/guide/serialization.html), MAVLink 2.0 truncates empty (zero-filled bytes) at the end of the payload; it will never truncate the first byte of the payload regardless of if it is empty or not. The grammar is thus defined such that it allows for each payload to be any length.


**A MAVLink 1.0 message begins with a message-specific message header which is 6 bytes in length.**

* Byte 0: Magic value/version (0xFE for MAVLink 1.0)
* Byte 1: Payload length
* Byte 2: Packet sequence
* Byte 3: System id
* Byte 4: Component id
* Byte 5: Message id

**A MAVLink 2.0 message also begins with a message-specific message header which is 10 bytes in length.**

* Byte 0: Magic value/version (0xFD for MAVLink 2.0)
* Byte 1: Payload length
* Byte 2: Incompatibility flag (0x01 indicates a signed message)
* Byte 3: Compatibility flag
* Byte 4: Packet sequence
* Byte 5: System id
* Byte 6: Component id
* Bytes 7 - 9: Message id

**One message type is defined as follows:**

* **HEARTBEAT**: The heartbeat message shows that a system or component is present and responding. The type and autopilot fields (along with the message component id), allow the receiving system to treat further messages from this system appropriately (e.g. by laying out the user interface based on the autopilot).
  * Bytes 0 - 3: *custom_mode*: a bitfield for use for autopilot-specific flags; `uint32_t`.
  * Byte 4: *type*: vehicle or component type. For a flight controller component the vehicle type (quadrotor, helicopter, etc.). For other components the component type (e.g. camera, gimbal, etc.). This should be used in preference to component id for identifying the component type; `uint8_t`.
  * Byte 5: *autopilot*: autopilot type/class; `uint8_t`.
  * Byte 6: *base_mode*; system mode bitmap; `uint8_t`.
  * Byte 7: *system_status*; system status flag; `uint8_t`.
  * Byte 8: *mavlink_version*; MAVLink version, not writable by user; `uint8_t_mavlink_version`.

All data packets are followed by a 2 byte message cyclic redundancy check which acts as a checksum.
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

## Messages in parser

* HEARTBEAT
* SYS\_STATUS
* SYSTEM\_TIME
* PING
* LINK\_NODE\_STATUS
* SET\_MODE
* PARAM\_REQUEST\_READ
* PARAM\_REQUEST\_LIST
* PARAM\_VALUE
* PARAM\_SET
