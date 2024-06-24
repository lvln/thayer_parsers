# MAVLink - generates a parser for the MAVLink grammar

## Directory structure

* **mavlink\_source\_files** holds 2 datafiles (*shortRun.pcap* and *longerRun.pcap*) containing data messages from 2 flights with a Holybro X500 V2 drone. These files were collected using Wireshark. This directory also holds C-code which can be used to clean the data as well as extract individual messages from the original files (`cleandata.c`).
	
## MAVLink
MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding gorund control stations.
The format of MAVLink messages is defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).
The data packets are found in packet capture, `.pcap`, files.

**`.pcap` files have the following format, as described [here](https://www.endace.com/learn/what-is-a-pcap-file)**
* A PCAP file header:
  * Bytes 0 - 3: Magic number.
	* If magic number = 0xA1B2C3D4 timestamp is seconds and microseconds. 
	* If magic number = 0xA1B23C4D, timestamp is seconds and nanoseconds.
  * Bytes 4 - 5: Major version = 2.
  * Bytes 6 - 7: Minor version = 4.
  * Bytes 8 - 15: Reserved.
  * Bytes 16 - 19: SnapLen - The SnapLen (snap length) is the maximum number of octets captured from each packet.
  * Bytes 20 - 21: Linkype - contains a value - assigned by [tcpdump.org](tcpdump.org) – that describes the type of link the packets were captured from.
  * Bytes 22 - 23: Frame check sequence.
* Each message is then followed by a 16 byte packet record header:
  * Bytes 0 - 3: Timestamp (seconds).
  * Bytes 4 - 7: Timestamp (microseconds or nanoseconds - depending on magic number in the PCAP file header).
  * Bytes 8 - 11: Captured packet length.
  * Bytes 12 - 15: Original packet length.
  
**The header for all MAVLink messages follows a standard format and is comprised of the first 32 bytes of the message (following the packer record header)**

* Bytes 0 - 3: Message family (big endian).
* Byte 4: 4 MSB are IP version; 4 LSB are header length.
* Byte 5: Differentiated service field.
* Bytes 6 - 7: Total length.
* Bytes 8 - 9: Identification.
* Bytes 10 - 11: Flags and fragment offset.
* Byte 12: Time to live.
* Byte 13: Protocol.
* Bytes 14 - 15: Header checksum.
* Bytes 16 - 19: Source address.
* Bytes 20 - 23: Destination address.
* Bytes 24 - 25: Source port.
* Bytes 26 - 27: Destination port.
* Bytes 28 - 29: Length.
* Bytes 30 - 31: Checksum.

**This is followed by a message-specific header which is 10 bytes in length**

* Byte 32: Magic value/version (always 0xfd for MAVLink 2.2).
* Byte 33: Payload length.
* Byte 34: Incompatibility flag.
* Byte 35: Compatibility flag.
* Byte 36: Packet sequence.
* Byte 37: System id.
* Byte 38: Component id.
* Bytes 39 - 41: Message id (big endian).

**A few messages in the MAVLink `common.xml` dialect:**
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
  
All data packets are followed by a 2 byte message cyclic redundancy check which acts as a checksum.
