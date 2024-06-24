# MAVLink - generates a parser for the MAVLink grammar

## Directory structure
	- **mavlink\_source\_files** holds 2 datafiles (*shortRun.pcap* and *longerRun.pcap*) containing data messages from 2 flights with a Holybro X500 V2 drone. These files were collected using Wireshark. This directory also holds C-code which can be used to clean the data as well as extract individual messages from the original files (`cleandata.c`).
	
## MAVLink
MAVLink is a lightweight messaging protocol which enables communication between drones and their corresponding gorund control stations.
The format of MAVLink messages is defined in the `common.xml` file and can be found [here](https://mavlink.io/en/messages/common.html).

**The header for all MAVLink messages follows a standard format and is comprised of the first 32 bytes of the message**
	- Bytes 0 - 3: Message family.
	- Byte 4: 4 MSB are IP version; 4 LSB are header length.
	- Byte 5: Differentiated service field.
	- Bytes 6 - 7: Total length.
	- Bytes 8 - 9: Identification.
	- Bytes 10 - 11: Flags and fragment offset.
	- Byte 12: Time to live.
	- Byte 13: Protocol.
	- Bytes 14 - 15: Header checksum.
	- Bytes 16 - 19: Source address.
	- Bytes 20 - 23: Destination address.
	- Bytes 24 - 25: Source port.
	- Bytes 26 - 27: Destination port.
	- Bytes 28 - 29: Length.
	- Bytes 30 - 31: Checksum.

**A few common messages in the MAVLink `common.xml` dialect:**
	- GLOBAL\_POSITION\_INT:
	The filtered global position (e.g. fused GPS and accelerometers). The position is in GPS-frame (right-handed, Z-up).
	

