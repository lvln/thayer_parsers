# MAVLink ATTITUDE, GLOBAL\_POSITION\_INT and SCALED_PRESSURE - generates a parser for grammar defined for MAVLink messages of type ATTITUDE, GLOBAL\_POSITION\_INT and SCALED\_PRESSURE

NOTE: This is a grammar for 3 MAVLink messages. It accepts zero or more of any of these messages in any combination.

All data packets were captured in `.pcap` files using Wireshark, a network protocol analyzer.

## Directory structure

* **xbnf** contains a working parser written in xbnf for an ATTITUDE message, a GLOBAL\_POSITION\_INT message, a conbination of the two, or no message at all.
* **tests** contains a full set of tests for the parser
  * `pass.1` is an empty file
  * `pass.2` is a single ATTITUDE message
  * `pass.3` is a single GLOBAL\_POSITION\_INT message
  * `pass.4` is a single SCALED\_PRESSURE message
  * `pass.5` is all of the ATTITUDE messages recorded during a short flight with the drone
  * `pass.6` is all of the GLOBAL\_POSITION\_INT messages recorded during a short flight with the drone
  * `pass.7` is all of the SCALED_PRESSURE messages recorded during a short flight with the drone
  * `pass.8` is a single ATTITUDE, SCALED_PRESSURE and GLOBAL\_POSITION\_INT message from a short flight with the drone
  * `pass.9` is all of the ATTITUDE, GLOBAL\_POSITION\_INT and SCALED_PRESSURE messages recorded during a short flight with the drone
  * `pass.10` - `pass.81` contains each of the messages from `pass.8` with a single field changed
  * `fail.1` is an ATTITUDE_QUATERNION message (incorrect messsage type)
  * `fail.2` - `fail.34` are messages with fields changed or additional bytes added/removed

## Data packet structure
MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding ground control stations.
The format of MAVLink messages is defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).

**Each message begins with a 16 byte packet record header:**
  
* Bytes 0 - 3: Timestamp (seconds).
* Bytes 4 - 7: Timestamp (microseconds or nanoseconds).
* Bytes 8 - 11: Captured packet length.
* Bytes 12 - 15: Original packet length.
  
**The header for each MAVLink message follows a standard format and is comprised of the first 32 bytes of the message (following the packer record header)**

* Bytes 0 - 3: Message family (always 0x00000002).
* Byte 4: 4 MSB are IP version ; 4 LSB are header length (0x45 for MAVLink messages).
* Byte 5: Differentiated service field.
* Bytes 6 - 7: Total length (big endian; 0x4400 for both ATTITUDE and GLOBAL\_POSITION\_INT)).
* Bytes 8 - 9: Identification.
* Bytes 10 - 11: Flags and fragment offset.
* Byte 12: Time to live.
* Byte 13: Protocol (always 0x11 for UDP).
* Bytes 14 - 15: Header checksum.
* Bytes 16 - 19: Source address.
* Bytes 20 - 23: Destination address.
* Bytes 24 - 25: Source port (big endian).
* Bytes 26 - 27: Destination port (big endian).
* Bytes 28 - 29: Length (big endian).
* Bytes 30 - 31: Checksum.

**This is followed by a message-specific header which is 10 bytes in length**

* Byte 32: Magic value/version (always 0xfd for MAVLink 2.0).
* Byte 33: Payload length (0x1C for ATTITUDE and GLOBAL\_POSITION\_INT).
* Byte 34: Incompatibility flag.
* Byte 35: Compatibility flag.
* Byte 36: Packet sequence.
* Byte 37: System id.
* Byte 38: Component id.
* Bytes 39 - 41: Message id.

**The three message types are defined as follows:**

* **GLOBAL_POSITION_INT**: The filtered global position (e.g. fused GPS and accelerometers). The position is in GPS-frame (right-handed, Z-up).
  * Bytes 42 - 45: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`.
  * Bytes 46 - 49: *lat*: latitude; *degE7*; `int32_t`.
  * Bytes 50 - 53: *lon*: longitude; *degE7*; `int32_t`.
  * Bytes 54 - 57: *alt*; altitude; *mm*; `int32_t`.
  * Bytes 58 - 61: *relative_alt*; altitude above ground; *mm*; `int32_t`.
  * Bytes 62 - 63: *vx*; ground x speed (latitude, positive north); *cm/s*; `int16_t`.
  * Bytes 64 - 65: *vy*; ground y speed (longitude, positive east); *cm/s*; `int16_t`.
  * Bytes 66 - 67: *vz*; ground z speed (altitude, positive down); *cm/s*; `int16_t`.
  * Bytes 68 - 69: *hdg*; vehicle heading (yaw angle); *cdeg*; `uint16_t`.
  
  ![GLOBAL_POSITION_INT](./.images/GPIImage.jpg)
  
* **ATTITUDE**: The attitude in the aeronautical frame (right-handed, Z-down, Y-right, X-front, ZYX, intrinsic)
  * Bytes 42 - 45: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`.
  * Bytes 46 - 49: *roll*; roll angle (-pi..+pi); *rad*; `float`.
  * Bytes 50 - 53: *pitch*; pitch angle (-pi..+pi); *rad*; `float`.
  * Bytes 54 - 57: *yaw*; yaw angle (-pi..+pi); *rad*; `float`.
  * Bytes 58 - 61: *rollspeed*; roll angular speed; *rad/s*; `float`.
  * Bytes 62 - 65: *pitchspeed*; pitch angular speed; *rad/s*; `float`.
  * Bytes 66 - 69: *yawspeed*; yaw angular speed; *rad/s*; `float`.
  
  ![GLOBAL_POSITION_INT](./.images/AttitudeImage.jpg)

* **SCALED_PRESSURE**: The pressure readings for the typical setup of one absolute and differential pressure sensor.
  * Bytes 42 - 45: *time_boot_ms*: the timestamp representing the time since system boot; *ms*; `uint32_t`.
  * Bytes 46 - 49: *press_abs*: absolute pressure; *hPa*; `float`.
  * Bytes 50 - 53: *press_diff*: differential pressure; *hPa*; `float`.
  * Bytes 54 - 55: *temperature*: absolute pressure temperature; *cdegC*; `int16_t`.
  * Bytes 56 - 57: *temperature_press_diff*: differential pressure temperature (0, if not available). Report values of 0 (or 1) as 1 cdegC.; *cdegC*; `int16_t`.

  ![SCALED_PRESSURE](./.images/SPImage.jpg)

All data packets are followed by a 2 byte message cyclic redundancy check which acts as a checksum.
