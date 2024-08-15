/* 
 * mavlink.c --- implementation of the functions described in mavlink.h
 * 
 * Author: Joshua M. Meise
 * Created: 06-27-2024
 * Version: 1.0
 * 
 * Description: Allows for data processing and test generation.
 * 
 */

// Library inclusions.
#include <stdio.h>
#include <mavlink.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

// Predefined constants.
#define HEARTBEAT 0
#define SYS_STATUS 1
#define SYSTEM_TIME 2
#define PING 4
#define LINK_NODE_STATUS 8
#define SET_MODE 11
#define PARAM_REQUEST_READ 20
#define PARAM_REQUEST_LIST 21
#define PARAM_VALUE 22
#define PARAM_SET 23
#define GPS_RAW_INT 24
#define SCALED_PRESSURE 29
#define ATTITUDE 30
#define ATTITUDE_QUATERNION 31
#define LOCAL_POSITION_NED 32
#define GLOBAL_POSITION_INT 33
#define MISSION_REQUEST 40
#define MISSION_CURRENT 42
#define MISSION_REQUEST_LIST 43
#define MISSION_COUNT 44
#define MISSION_CLEAR_ALL 45
#define MISSION_ITEM_REACHED 46
#define MISSION_ACK 47
#define GPS_GLOBAL_ORIGIN 49
#define MISSION_REQUEST_INT 51
#define RC_CHANNELS 65
#define MANUAL_CONTROL 69
#define MISSION_ITEM_INT 73
#define VFR_HUD 74
#define COMMAND_LONG 76
#define COMMAND_ACK 77
#define ATTITUDE_TARGET 83
#define POSITION_TARGET_LOCAL_NED 85
#define POSITION_TARGET_GLOBAL_INT 87
#define RADIO_STATUS 109
#define DISTANCE_SENSOR 132
#define ALTITUDE 141
#define BATTERY_STATUS 147
#define AUTOPILOT_VERSION 148
#define ESTIMATOR_STATUS 230
#define VIBRATION 241
#define HOME_POSITION 242
#define EXTENDED_SYS_STATE 245
#define STATUSTEXT 253
#define PROTOCOL_VERSION 300
#define UTM_GLOBAL_POSITION 340
#define TIME_ESTIMATE_TO_TARGET 380
#define COMPONENT_METADATA 397
#define EVENT 410
#define CURRENT_EVENT_SEQUENCE 411
#define OPEN_DRONE_ID_LOCATION 12901
#define OPEN_DRONE_ID_SYSTEM 12904

#define HEARTBEAT_LEN 9
#define SYS_STATUS_LEN 43
#define SYSTEM_TIME_LEN 12
#define PING_LEN 14
#define LINK_NODE_STATUS_LEN 36
#define SET_MODE_LEN 6
#define PARAM_REQUEST_READ_LEN 22
#define PARAM_REQUEST_LIST_LEN 2
#define PARAM_VALUE_LEN 25
#define PARAM_SET_LEN 23
#define GPS_RAW_INT_LEN 52
#define SCALED_PRESSURE_LEN 16
#define ATTITUDE_LEN 28
#define ATTITUDE_QUATERNION_LEN 48
#define LOCAL_POSITION_NED_LEN 28
#define GLOBAL_POSITION_INT_LEN 28
#define MISSION_REQUEST_LEN 5
#define MISSION_CURRENT_LEN 18
#define MISSION_REQUEST_LIST_LEN 3
#define MISSION_COUNT_LEN 9
#define MISSION_CLEAR_ALL_LEN 3
#define MISSION_ITEM_REACHED_LEN 2
#define MISSION_ACK_LEN 8 
#define GPS_GLOBAL_ORIGIN_LEN 20
#define MISSION_REQUEST_INT_LEN 5
#define RC_CHANNELS_LEN 42
#define MANUAL_CONTROL_LEN 30
#define MISSION_ITEM_INT_LEN 38
#define VFR_HUD_LEN 20
#define COMMAND_LONG_LEN 33
#define COMMAND_ACK_LEN 10
#define ATTITUDE_TARGET_LEN 37
#define POSITION_TARGET_LOCAL_NED_LEN 51
#define POSITION_TARGET_GLOBAL_INT_LEN 51
#define RADIO_STATUS_LEN 9
#define DISTANCE_SENSOR_LEN 39
#define ALTITUDE_LEN 32
#define BATTERY_STATUS_LEN 54
#define AUTOPILOT_VERSION_LEN 78
#define ESTIMATOR_STATUS_LEN 42
#define VIBRATION_LEN 32
#define HOME_POSITION_LEN 60
#define EXTENDED_SYS_STATE_LEN 2
#define STATUSTEXT_LEN 54
#define PROTOCOL_VERSION_LEN 22
#define UTM_GLOBAL_POSITION_LEN 70
#define TIME_ESTIMATE_TO_TARGET_LEN 20
#define COMPONENT_METADATA_LEN 108
#define EVENT_LEN 53
#define CURRENT_EVENT_SEQUENCE_LEN 3
#define OPEN_DRONE_ID_LOCATION_LEN 59
#define OPEN_DRONE_ID_SYSTEM_LEN 54

// Packet record header
typedef struct pcaketRecordHeader {
	uint8_t timeS[4];
	uint8_t timeUNS[4];
	uint8_t capturedPacketLength[4];
	uint8_t originalPacketLength[4];
} packetRecordHeader_t;

// Ethernet header.
typedef struct ethernetHeader {
	uint8_t messageFam[4];
	uint8_t iphl;
	uint8_t dsf;
	uint8_t totalLength[2];
	uint8_t id[2];
	uint8_t ffOffset[2];
	uint8_t ttl;
	uint8_t protocol;
	uint8_t headerChecksum[2];
	uint8_t sourceAddr[4];
	uint8_t destAddr[4];
	uint8_t sourcePort[2];
	uint8_t destPort[2];
	uint8_t length[2];
	uint8_t checkSum[2];
} ethernetHeader_t;

// TLOG header.
typedef struct tlogHeader {
	uint8_t bytes[8];
} tlogHeader_t;

// MAVLink 1 message.
typedef struct mav1Message {
	uint8_t mavCode;
	uint8_t payloadLen;
	uint8_t packetSeq;
	uint8_t systemID;
	uint8_t compID;
	uint8_t messageID;
	uint8_t *payload;
	uint8_t crc[2];
} mav1Message_t;

// MAVLink 2 message.
typedef struct mav2Message {
	uint8_t mavCode;
	uint8_t payloadLen;
	uint8_t incompFlag;
	uint8_t compFlag;
	uint8_t packetSeq;
	uint8_t systemID;
	uint8_t compID;
	uint8_t messageID[3];
	uint8_t *payload;
	uint8_t crc[2];
	bool signedMess;
	uint8_t signature[13];
} mav2Message_t;

// MAVLink message is either MAVLink 1 or MAVLink 2.
typedef union mavMessage {
	mav1Message_t mav1;
	mav2Message_t mav2;
} mavMessage_t;

// A TLOG message contains a tlog header and a MAVLink message.
typedef struct tlogMessage {
	tlogHeader_t header;
	mavMessage_t mav;
} tlogMessage_t;

// A pcap message contains a packet record header, an ethernet header and a MAVLink message.
typedef struct pcapMessage {
	packetRecordHeader_t prh;
	ethernetHeader_t eth;
	mavMessage_t mav;
} pcapMessage_t;

// Data structure to hold a TLOG file.
typedef struct tlog {
	vector_t *messages;
} pTlog_t;

// Data structure to hold a MAVLink file.
typedef struct mavlink {
	vector_t *messages;
} pMavlink_t;

// Data structure to hold a pcap file.
typedef struct pcap {
	uint8_t header[24];
	vector_t *messages;
} pPcap_t;


// Global variables.
static FILE *file;

/*
 * Checks if 2 integers have the same value.
 * Inputs: pointers to integers to compare
 * Outputs: true if same, false otherwise
 */
static bool compareInt(void *vp1, void *vp2) {
	// Variable declarations
	int *i, *j;

	// Coerce into valid datatypes
	i = (int *)vp1;
	j = (int *)vp2;

	// Compare
	if (*i == *j) return true;
	else return false;
}

/*
 * Checks if 2 8 bit integers have the same value.
 * Inputs: pointers to integers to compare
 * Outputs: true if same, false otherwise
 */
static bool compareInt8(void *vp1, void *vp2) {
	// Variable declarations
	uint8_t *i, *j;

	// Coerce into valid datatypes
	i = (uint8_t *)vp1;
	j = (uint8_t *)vp2;

	// Compare
	if (*i == *j) return true;
	else return false;
}

/*
 * Print out a message id.
 * Inputs: pointer to ID to print
 * Outputs: none
 */
static void printID(void *idp) {
	// Variable declarations.
	int *id;

	// Coerce.
	id = (int *)idp;

	printf("Message ID: %d\n", *id);
}

/*
 * Convert from integer to 16 bit hexadecimal (little endian).
 * Inputs: integer value, array in which to store hex number
 * Outputs: none
 */

static void fromInt16be(int num, uint8_t arr[]) {
	arr[1] = num % 256;
	num /= 256;
	arr[0] = num % 256;
	
}

/*
 * Convert from integer to 24 bit hexadecimal (little endian).
 * Inputs: integer value, array in which to store hex number
 * Outputs: none
 */
static void fromInt24le(int num, uint8_t arr[]) {
	arr[0] = num % 256;
	num /= 256;
	arr[1] = num % 256;
	num /= 256;
	arr[2] = num % 256;

}

/*
 * Convert from integer to 32 bit hexadecimal (little endian).
 * Inputs: integer value, array in which to store hex number
 * Outputs: none
 */
static void fromInt32le(int num, uint8_t arr[]) {
	arr[0] = num % 256;
	num /= 256;
	arr[1] = num % 256;
	num /= 256;
	arr[2] = num % 256;
	num /= 256;
	arr[3] = num % 256;
	
}

/*
 * Convert from 24 bit hexadecimal (little endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
static int toInt24le(uint8_t *arr) {
	return (int)arr[0] + (int)arr[1]*256 + (int)arr[2]*4096;
}

/*
 * Print out a message including its PCAP wrappers.
 * Inputs: message to print
 * Outputs: none.
 */
static void printMessagePcap(pcapMessage_t *mess) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;
	bool mav1, mav2;

	// Check argument.
	if (mess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	len = toInt24le(mess->prh.capturedPacketLength) + 16;
	mav1 = mav2 = false;
	
	// Detect type of message.
	if (mess->mav.mav1.mavCode == 0xfe) mav1 = true;
	else if (mess->mav.mav2.mavCode == 0xfd) mav2 = true;

	printf("Message: \n" );
	
	for (i = 0; i < len; i++) {
		if (i < 4) buf = mess->prh.timeS[i];
		else if (i < 8) buf = mess->prh.timeUNS[i - 4];
		else if (i < 12) buf = mess->prh.capturedPacketLength[i - 8];
		else if (i < 16) buf = mess->prh.originalPacketLength[i - 12];
		else if (i < 20) buf = mess->eth.messageFam[i - 16];
		else if (i == 20) buf = mess->eth.iphl;
		else if (i == 21) buf = mess->eth.dsf;
		else if (i < 24) buf = mess->eth.totalLength[i - 22];
		else if (i < 26) buf = mess->eth.id[i - 24];
		else if (i < 28) buf = mess->eth.ffOffset[i - 26];
		else if (i == 28) buf = mess->eth.ttl;
		else if (i == 29) buf = mess->eth.protocol;
		else if (i < 32) buf = mess->eth.headerChecksum[i - 30];
		else if (i < 36) buf = mess->eth.sourceAddr[i - 32];
		else if (i < 40) buf = mess->eth.destAddr[i - 36];
		else if (i < 42) buf = mess->eth.sourcePort[i - 40];
		else if (i < 44) buf = mess->eth.destPort[i - 42];
		else if (i < 46) buf = mess->eth.length[i - 44];
		else if (i < 48) buf = mess->eth.checkSum[i - 46];
	
		// Depending on the messsage type, set the respective fields.
		if (mav1 && i >= 48) {
			if (i == 48) buf = mess->mav.mav1.mavCode;
			else if (i == 49) {
				buf = mess->mav.mav1.payloadLen;

				payload = (int)buf;

			}
			else if (i == 50) buf =  mess->mav.mav1.packetSeq;
			else if (i == 51) buf = mess->mav.mav1.systemID;
			else if (i == 52) buf = mess->mav.mav1.compID;
			else if (i == 53) buf = mess->mav.mav1.messageID;		
			else if (i < 54 + payload) buf = mess->mav.mav1.payload[i - 54];
			else buf = mess->mav.mav1.crc[i - 54 - payload];		
		}
		else if (mav2 && i >= 48) {
			if (i == 48) buf = mess->mav.mav2.mavCode;
			else if (i == 49) {
				buf = mess->mav.mav2.payloadLen;

				payload = (int)buf;
			}
			else if (i == 50) buf = mess->mav.mav2.incompFlag;
			else if (i == 51) buf = mess->mav.mav2.compFlag;
			else if (i == 52) buf = mess->mav.mav2.packetSeq;
			else if (i == 53) buf = mess->mav.mav2.systemID;
			else if (i == 54) buf = mess->mav.mav2.compID;
			else if (i < 58) buf = mess->mav.mav2.messageID[i - 55];		
			else if (i < 58 + payload) buf = mess->mav.mav2.payload[i - 58];
			else if (i < 60 + payload) buf = mess->mav.mav2.crc[i - 58 - payload];
			else buf = mess->mav.mav2.signature[i - 60 - payload];
		}

		printf("%02x ", buf);

	}

	printf("\n");
}

/*
 * Print out a MAVLink message.
 * Inputs: message to print
 * Outputs: none
 */
static void printMessageMav(mavMessage_t *mess) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;
	bool mav1, mav2;

	// Check argument.
	if (mess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	len = 0;
	mav1 = mav2 = false;
	
	// Detect type of message.
	if (mess->mav1.mavCode == 0xfe) {
		mav1 = true;
		len = (int)mess->mav1.payloadLen + 8;
	}
	else if (mess->mav2.mavCode == 0xfd) {
		mav2 = true;
		len = (int)mess->mav2.payloadLen + 12;
		if (mess->mav2.signedMess) len += 13;
	}

	printf("Message: \n" );
	
	for (i = 0; i < len; i++) {
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) buf = mess->mav1.mavCode;
			else if (i == 1) {
				buf = mess->mav1.payloadLen;

				payload = (int)buf;

			}
			else if (i == 2) buf = mess->mav1.packetSeq;
			else if (i == 3) buf = mess->mav1.systemID;
			else if (i == 4) buf = mess->mav1.compID;
			else if (i == 5) buf = mess->mav1.messageID;		
			else if (i < 6 + payload) buf = mess->mav1.payload[i - 6];
			else buf = mess->mav1.crc[i - 6 - payload];		
		}
		else if (mav2) {
			if (i == 0) buf = mess->mav2.mavCode;
			else if (i == 1) {
				buf = mess->mav2.payloadLen;

				payload = (int)buf;
			}
			else if (i == 2) buf = mess->mav2.incompFlag;
			else if (i == 3) buf = mess->mav2.compFlag;
			else if (i == 4) buf = mess->mav2.packetSeq;
			else if (i == 5) buf = mess->mav2.systemID;
			else if (i == 6) buf = mess->mav2.compID;
			else if (i < 10) buf = mess->mav2.messageID[i - 7];		
			else if (i < 10 + payload) buf = mess->mav2.payload[i - 10];
			else if (i < 12 + payload) buf = mess->mav2.crc[i - 10 - payload];
			else buf = mess->mav2.signature[i - 12 - payload];
		}

		printf("%02x ", buf);

	}

	printf("\n");
}

/*
 * Print out a TLOG message.
 * Inputs: message to print
 * Outputs: none
 */
static void printMessageTlog(tlogMessage_t *mess) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;
	bool mav1, mav2;

	// Check argument.
	if (mess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}
	
	len = 0;
	mav1 = mav2 = false;

	// Detect type of message.
	if (mess->mav.mav1.mavCode == 0xfe) {
		mav1 = true;
		len = (int)mess->mav.mav1.payloadLen + 16;
	}
	else if (mess->mav.mav2.mavCode == 0xfd) {
		mav2 = true;
		len = (int)mess->mav.mav2.payloadLen + 20;
		if (mess->mav.mav2.signedMess) len += 13;
	}

	printf("Message: \n" );
	
	for (i = 0; i < len; i++) {
		if (i < 8) buf = mess->header.bytes[i];
								 
		// Depending on the messsage type, set the respective fields.
		if (mav1 && i >= 8) {
			if (i == 8) buf = mess->mav.mav1.mavCode;
			else if (i == 9) {
				buf = mess->mav.mav1.payloadLen;

				payload = (int)buf;

			}
			else if (i == 10) buf =  mess->mav.mav1.packetSeq;
			else if (i == 11) buf = mess->mav.mav1.systemID;
			else if (i == 12) buf = mess->mav.mav1.compID;
			else if (i == 13) buf = mess->mav.mav1.messageID;		
			else if (i < 14 + payload) buf = mess->mav.mav1.payload[i - 14];
			else buf = mess->mav.mav1.crc[i - 14 - payload];		
		}
		else if (mav2 && i >= 8) {
			if (i == 8) buf = mess->mav.mav2.mavCode;
			else if (i == 9) {
				buf = mess->mav.mav2.payloadLen;

				payload = (int)buf;
			}
			else if (i == 10) buf = mess->mav.mav2.incompFlag;
			else if (i == 11) buf = mess->mav.mav2.compFlag;
			else if (i == 12) buf = mess->mav.mav2.packetSeq;
			else if (i == 13) buf = mess->mav.mav2.systemID;
			else if (i == 14) buf = mess->mav.mav2.compID;
			else if (i < 18) buf = mess->mav.mav2.messageID[i - 15];		
			else if (i < 18 + payload) buf = mess->mav.mav2.payload[i - 18];
			else if (i < 20 + payload) buf = mess->mav.mav2.crc[i - 18 - payload];
			else buf = mess->mav.mav2.signature[i - 20 - payload];
		}

		printf("%02x ", buf);

	}

	printf("\n");
}

/*
 * Write a PCAP message to a file.
 * Inputs: message to write, file to write to
 * Outputs: none
 */
static void writeMessageToFilePcap(pcapMessage_t *mess, FILE *fp) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;
	bool mav1, mav2;

	// Check arguments.
	if (mess == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}
	
	len = toInt24le(mess->prh.capturedPacketLength) + 16;
	mav1 = mav2 = false;
	
	// Detect type of message.
	if (mess->mav.mav1.mavCode == 0xfe) mav1 = true;
	else if (mess->mav.mav2.mavCode == 0xfd) mav2 = true;
	
	for (i = 0; i < len; i++) {
		if (i < 4) buf = mess->prh.timeS[i];
		else if (i < 8) buf = mess->prh.timeUNS[i - 4];
		else if (i < 12) buf = mess->prh.capturedPacketLength[i - 8];
		else if (i < 16) buf = mess->prh.originalPacketLength[i - 12];
		else if (i < 20) buf = mess->eth.messageFam[i - 16];
		else if (i == 20) buf = mess->eth.iphl;
		else if (i == 21) buf = mess->eth.dsf;
		else if (i < 24) buf = mess->eth.totalLength[i - 22];
		else if (i < 26) buf = mess->eth.id[i - 24];
		else if (i < 28) buf = mess->eth.ffOffset[i - 26];
		else if (i == 28) buf = mess->eth.ttl;
		else if (i == 29) buf = mess->eth.protocol;
		else if (i < 32) buf = mess->eth.headerChecksum[i - 30];
		else if (i < 36) buf = mess->eth.sourceAddr[i - 32];
		else if (i < 40) buf = mess->eth.destAddr[i - 36];
		else if (i < 42) buf = mess->eth.sourcePort[i - 40];
		else if (i < 44) buf = mess->eth.destPort[i - 42];
		else if (i < 46) buf = mess->eth.length[i - 44];
		else if (i < 48) buf = mess->eth.checkSum[i - 46];
	
		// Depending on the messsage type, set the respective fields.
		if (mav1 && i >= 48) {
			if (i == 48) buf = mess->mav.mav1.mavCode;
			else if (i == 49) {
				buf = mess->mav.mav1.payloadLen;

				payload = (int)buf;

			}
			else if (i == 50) buf =  mess->mav.mav1.packetSeq;
			else if (i == 51) buf = mess->mav.mav1.systemID;
			else if (i == 52) buf = mess->mav.mav1.compID;
			else if (i == 53) buf = mess->mav.mav1.messageID;		
			else if (i < 54 + payload) buf = mess->mav.mav1.payload[i - 54];
			else buf = mess->mav.mav1.crc[i - 54 - payload];		
		}
		else if (mav2 && i >= 48) {
			if (i == 48) buf = mess->mav.mav2.mavCode;
			else if (i == 49) {
				buf = mess->mav.mav2.payloadLen;

				payload = (int)buf;
			}
			else if (i == 50) buf = mess->mav.mav2.incompFlag;
			else if (i == 51) buf = mess->mav.mav2.compFlag;
			else if (i == 52) buf = mess->mav.mav2.packetSeq;
			else if (i == 53) buf = mess->mav.mav2.systemID;
			else if (i == 54) buf = mess->mav.mav2.compID;
			else if (i < 58) buf = mess->mav.mav2.messageID[i - 55];		
			else if (i < 58 + payload) buf = mess->mav.mav2.payload[i - 58];
			else if (i < 60 + payload) buf = mess->mav.mav2.crc[i - 58 - payload];
			else buf = mess->mav.mav2.signature[i - 60 - payload];
		}

		if (fwrite(&buf, sizeof(buf), 1, fp) != 1)
			fprintf(stderr, "Error writing byte %02x to file.\n", buf);

	}
}

/*
 * Write a MAVLink message to a file.
 * Inputs: message to write, file to write to
 * Outputs: none
 */
static void writeMessageToFileMav(mavMessage_t *mess, FILE *fp) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;
	bool mav1, mav2;

	// Check arguments.
	if (mess == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}
	
	len = 0;
	mav1 = mav2 = false;
	
	// Detect type of message.
	if (mess->mav1.mavCode == 0xfe) {
		mav1 = true;
		len = (int)mess->mav1.payloadLen + 8;
	}
	else if (mess->mav2.mavCode == 0xfd) {
		mav2 = true;
		len = (int)mess->mav2.payloadLen + 12;
		if (mess->mav2.signedMess) len += 13;
	}

	for (i = 0; i < len; i++) {
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) buf = mess->mav1.mavCode;
			else if (i == 1) {
				buf = mess->mav1.payloadLen;

				payload = (int)buf;

			}
			else if (i == 2) buf = mess->mav1.packetSeq;
			else if (i == 3) buf = mess->mav1.systemID;
			else if (i == 4) buf = mess->mav1.compID;
			else if (i == 5) buf = mess->mav1.messageID;		
			else if (i < 6 + payload) buf = mess->mav1.payload[i - 6];
			else buf = mess->mav1.crc[i - 6 - payload];		
		}
		else if (mav2) {
			if (i == 0) buf = mess->mav2.mavCode;
			else if (i == 1) {
				buf = mess->mav2.payloadLen;

				payload = (int)buf;
			}
			else if (i == 2) buf = mess->mav2.incompFlag;
			else if (i == 3) buf = mess->mav2.compFlag;
			else if (i == 4) buf = mess->mav2.packetSeq;
			else if (i == 5) buf = mess->mav2.systemID;
			else if (i == 6) buf = mess->mav2.compID;
			else if (i < 10) buf = mess->mav2.messageID[i - 7];		
			else if (i < 10 + payload) buf = mess->mav2.payload[i - 10];
			else if (i < 12 + payload) buf = mess->mav2.crc[i - 10 - payload];
			else buf = mess->mav2.signature[i - 12 - payload];
		}

		if (fwrite(&buf, sizeof(buf), 1, fp) != 1)
			fprintf(stderr, "Error writing byte %02x to file.\n", buf);

	}
}

/*
 * Write a MAVLink message to a file for which a a format specifying field has been altered.
 * Inputs: message to write, file to write to, MAVLink version (1 or 2), actual payload length, number of bytes to write
 * Outputs: none
 */
static void writeAltFormatMessageToFileMav(mavMessage_t *mess, FILE *fp, int mavVersion, int payload, int numBytes) {
	// Variable declarations.
	int i;
	uint8_t buf;
	bool mav1, mav2;

	// Check arguments.
	if (mess == NULL || fp == NULL || !(mavVersion == 1 || mavVersion == 2)) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}
	
	mav1 = mav2 = false;
	
	// Detect type of message.
	if (mavVersion == 1) mav1 = true;
	else if (mavVersion == 2)	mav2 = true;

	for (i = 0; i < numBytes; i++) {
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) buf = mess->mav1.mavCode;
			else if (i == 1) buf = mess->mav1.payloadLen;
			else if (i == 2) buf = mess->mav1.packetSeq;
			else if (i == 3) buf = mess->mav1.systemID;
			else if (i == 4) buf = mess->mav1.compID;
			else if (i == 5) buf = mess->mav1.messageID;		
			else if (i < 6 + payload) buf = mess->mav1.payload[i - 6];
			else if (i < 8 + payload) buf = mess->mav1.crc[i - 6 - payload];
			else buf = (uint8_t)(rand() % 256);
		}
		else if (mav2) {
			if (i == 0) buf = mess->mav2.mavCode;
			else if (i == 1) buf = mess->mav2.payloadLen;
			else if (i == 2) buf = mess->mav2.incompFlag;
			else if (i == 3) buf = mess->mav2.compFlag;
			else if (i == 4) buf = mess->mav2.packetSeq;
			else if (i == 5) buf = mess->mav2.systemID;
			else if (i == 6) buf = mess->mav2.compID;
			else if (i < 10) buf = mess->mav2.messageID[i - 7];		
			else if (i < 10 + payload) buf = mess->mav2.payload[i - 10];
			else if (i < 12 + payload) buf = mess->mav2.crc[i - 10 - payload];
			else if (i < 14 + payload && mess->mav2.signedMess) buf = mess->mav2.signature[i - 12 - payload];
			else buf = (uint8_t)(rand() % 256);
		}

		if (fwrite(&buf, sizeof(buf), 1, fp) != 1)
			fprintf(stderr, "Error writing byte %02x to file.\n", buf);

	}
}

/*
 * Write a TLOG message to a file.
 * Inputs: message to write, file to write to
 * Outputs: none
 */
static void writeMessageToFileTlog(tlogMessage_t *mess, FILE *fp) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;
	bool mav1, mav2;

	// Check argument.
	if (mess == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}
	
	len = 0;
	mav1 = mav2 = false;
	
	// Detect type of message.
	if (mess->mav.mav1.mavCode == 0xfe) {
		mav1 = true;
		len = (int)mess->mav.mav1.payloadLen + 16;
	}
	else if (mess->mav.mav2.mavCode == 0xfd) {
		mav2 = true;
		len = (int)mess->mav.mav2.payloadLen + 20;
		if (mess->mav.mav2.signedMess) len += 13;
	}

	for (i = 0; i < len; i++) {
		if (i < 8) buf = mess->header.bytes[i];
								 
		// Depending on the messsage type, set the respective fields.
		if (mav1 && i >= 8) {
			if (i == 8) buf = mess->mav.mav1.mavCode;
			else if (i == 9) {
				buf = mess->mav.mav1.payloadLen;

				payload = (int)buf;

			}
			else if (i == 10) buf =  mess->mav.mav1.packetSeq;
			else if (i == 11) buf = mess->mav.mav1.systemID;
			else if (i == 12) buf = mess->mav.mav1.compID;
			else if (i == 13) buf = mess->mav.mav1.messageID;		
			else if (i < 14 + payload) buf = mess->mav.mav1.payload[i - 14];
			else buf = mess->mav.mav1.crc[i - 14 - payload];		
		}
		else if (mav2 && i >= 8) {
			if (i == 8) buf = mess->mav.mav2.mavCode;
			else if (i == 9) {
				buf = mess->mav.mav2.payloadLen;

				payload = (int)buf;
			}
			else if (i == 10) buf = mess->mav.mav2.incompFlag;
			else if (i == 11) buf = mess->mav.mav2.compFlag;
			else if (i == 12) buf = mess->mav.mav2.packetSeq;
			else if (i == 13) buf = mess->mav.mav2.systemID;
			else if (i == 14) buf = mess->mav.mav2.compID;
			else if (i < 18) buf = mess->mav.mav2.messageID[i - 15];		
			else if (i < 18 + payload) buf = mess->mav.mav2.payload[i - 18];
			else buf = mess->mav.mav2.crc[i - 18 - payload];		
		}

		if (fwrite(&buf, sizeof(buf), 1, fp) != 1)
			fprintf(stderr, "Error writing byte %02x to file.\n", buf);
	}

}

/*
 * Convert a TLOG message to a MAVLink message.
 * Inputs: TLOG message
 * Outputs: MAVLink message; NULL if unsuccessful
 */
static mavMessage_t *tlogMessToMav(tlogMessage_t *tMess) {
	// Varaible declarations.
	mavMessage_t *mess;
	int i, len, payload;
	bool mav1, mav2;

	// Check arguments.
	if (tMess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	len = 0;
	mav1 = mav2 = false;
	
	// Check type of message.
	if (tMess->mav.mav1.mavCode == 0xfe) {
		mav1 = true;
		len = tMess->mav.mav1.payloadLen + 8;
	}
	else if (tMess->mav.mav2.mavCode == 0xfd) {
		mav2 = true;
		len = tMess->mav.mav2.payloadLen + 12;
		if (tMess->mav.mav2.signedMess) len += 13;
	}
		
	// Allocate space for new MAVlink message.
	if ((mess = (mavMessage_t *)malloc(sizeof(mavMessage_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Copy over all of the fields.
	for (i = 0; i < len; i++) {
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) mess->mav1.mavCode = tMess->mav.mav1.mavCode;
			else if (i == 1) {
				mess->mav1.payloadLen = tMess->mav.mav1.payloadLen;
				
				payload = (int)mess->mav1.payloadLen;

				if ((mess->mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}				
				
			}
			else if (i == 2) mess->mav1.packetSeq = tMess->mav.mav1.packetSeq;
			else if (i == 3) mess->mav1.systemID = tMess->mav.mav1.systemID;
			else if (i == 4) mess->mav1.compID = tMess->mav.mav1.compID;
			else if (i == 5) mess->mav1.messageID = tMess->mav.mav1.messageID;
			else if (i < 6 + payload) mess->mav1.payload[i - 6] = tMess->mav.mav1.payload[i - 6];
			else mess->mav1.crc[i - 6 - payload] = tMess->mav.mav1.crc[i - 6 - payload];		
		}
		else if (mav2) {
			if (i == 0) {
				mess->mav2.mavCode = tMess->mav.mav2.mavCode;
				mess->mav2.signedMess = tMess->mav.mav2.signedMess;
			}
			else if (i == 1) {
				mess->mav2.payloadLen = tMess->mav.mav2.payloadLen;

				payload = (int)mess->mav2.payloadLen;
				
				if ((mess->mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 2) mess->mav2.incompFlag = tMess->mav.mav2.incompFlag;
			else if (i == 3) mess->mav2.compFlag = tMess->mav.mav2.compFlag;
			else if (i == 4) mess->mav2.packetSeq = tMess->mav.mav2.packetSeq;
			else if (i == 5) mess->mav2.systemID = tMess->mav.mav2.systemID;
			else if (i == 6) mess->mav2.compID = tMess->mav.mav2.compID;
			else if (i < 10) mess->mav2.messageID[i - 7] = tMess->mav.mav2.messageID[i - 7];
			else if (i < 10 + payload) mess->mav2.payload[i - 10] = tMess->mav.mav2.payload[i - 10];
			else if (i < 12 + payload) mess->mav2.crc[i - 10 - payload] = tMess->mav.mav2.crc[i - 10 - payload];
			else mess->mav2.signature[i - 12 - payload] = tMess->mav.mav2.signature[i - 12 - payload];
		}
	}
	return mess;
}

/*
 * Make a copyof a MAVLink message.
 * Inputs: MAVLink message
 * Outputs: MAVLink message; NULL if unsuccessful
 */
static mavMessage_t *mavMessToMav(mavMessage_t *mMess) {
	// Varaible declarations.
	mavMessage_t *mess;
	int i, len, payload;
	bool mav1, mav2;

	// Check arguments.
	if (mMess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	len = 0;
	mav1 = mav2 = false;
	
	// Check type of message.
	if (mMess->mav1.mavCode == 0xfe) {
		mav1 = true;
		len = mMess->mav1.payloadLen + 8;
	}
	else if (mMess->mav2.mavCode == 0xfd) {
		mav2 = true;
		len = mMess->mav2.payloadLen + 12;
		if (mMess->mav2.signedMess) len += 13;
	}
		
	// Allocate space for new MAVlink message.
	if ((mess = (mavMessage_t *)malloc(sizeof(mavMessage_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Copy over all of the fields.
	for (i = 0; i < len; i++) {
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) mess->mav1.mavCode = mMess->mav1.mavCode;
			else if (i == 1) {
				mess->mav1.payloadLen = mMess->mav1.payloadLen;
				
				payload = (int)mess->mav1.payloadLen;

				if ((mess->mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}				
				
			}
			else if (i == 2) mess->mav1.packetSeq = mMess->mav1.packetSeq;
			else if (i == 3) mess->mav1.systemID = mMess->mav1.systemID;
			else if (i == 4) mess->mav1.compID = mMess->mav1.compID;
			else if (i == 5) mess->mav1.messageID = mMess->mav1.messageID;
			else if (i < 6 + payload) mess->mav1.payload[i - 6] = mMess->mav1.payload[i - 6];
			else mess->mav1.crc[i - 6 - payload] = mMess->mav1.crc[i - 6 - payload];		
		}
		else if (mav2) {
			if (i == 0) {
				mess->mav2.mavCode = mMess->mav2.mavCode;
				mess->mav2.signedMess = mMess->mav2.signedMess;
			}
			else if (i == 1) {
				mess->mav2.payloadLen = mMess->mav2.payloadLen;

				payload = (int)mess->mav2.payloadLen;
				
				if ((mess->mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 2) mess->mav2.incompFlag = mMess->mav2.incompFlag;
			else if (i == 3) mess->mav2.compFlag = mMess->mav2.compFlag;
			else if (i == 4) mess->mav2.packetSeq = mMess->mav2.packetSeq;
			else if (i == 5) mess->mav2.systemID = mMess->mav2.systemID;
			else if (i == 6) mess->mav2.compID = mMess->mav2.compID;
			else if (i < 10) mess->mav2.messageID[i - 7] = mMess->mav2.messageID[i - 7];
			else if (i < 10 + payload) mess->mav2.payload[i - 10] = mMess->mav2.payload[i - 10];
			else if (i < 12 + payload) mess->mav2.crc[i - 10 - payload] = mMess->mav2.crc[i - 10 - payload];
			else mess->mav2.signature[i - 12 - payload] = mMess->mav2.signature[i - 12 - payload];
		}
	}
	return mess;
}

/*
 * Convert a TLOG message to a PCAP message.
 * Inputs: TLOG message
 * Outputs: PCAP message; NULL if unsuccessful
 */
static pcapMessage_t *tlogMessToPcap(tlogMessage_t *tMess) {
	// Variable declarations.
	int len, mavLen, i, payload;
	bool mav1, mav2;
	pcapMessage_t *mess;
	struct timeval tv;
	
	// Check arguments.
	if (tMess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	len = 0;
	mav1 = mav2 = false;
	
	// Check type of message.
	if (tMess->mav.mav1.mavCode == 0xfe) {
		mav1 = true;
		len = tMess->mav.mav1.payloadLen + 8;
	}
	else if (tMess->mav.mav2.mavCode == 0xfd) {
		mav2 = true;
		len = tMess->mav.mav2.payloadLen + 12;
		if (tMess->mav.mav2.signedMess) len += 13;
	}
		
	// Allocate space for new MAVlink message.
	if ((mess = (pcapMessage_t *)malloc(sizeof(pcapMessage_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Get the time stamp and insert it.
	gettimeofday(&tv, NULL);
	fromInt32le(tv.tv_sec, mess->prh.timeS);
	fromInt32le(tv.tv_usec, mess->prh.timeUNS);

	// Set length.
	if (mav1) len = tMess->mav.mav1.payloadLen + 40;
	else if (mav2) len = tMess->mav.mav2.payloadLen + 44;

	fromInt32le(len, mess->prh.capturedPacketLength);
	fromInt32le(len, mess->prh.originalPacketLength);
	
	// Fill in ethernet header.
	fromInt32le(2, mess->eth.messageFam);
	mess->eth.iphl = 0x45;
	mess->eth.dsf = 0;
	fromInt16be(len - 4, mess->eth.totalLength);
	mess->eth.id[0] = 0;
	mess->eth.id[1] = 0;
	mess->eth.ffOffset[0] = 0;
	mess->eth.ffOffset[1] = 0;
	mess->eth.ttl = 0x40;
	mess->eth.protocol = 0x11;
	mess->eth.headerChecksum[0] = 0;
	mess->eth.headerChecksum[1] = 0;
	mess->eth.sourceAddr[0] = 0x7f;
	mess->eth.sourceAddr[1] = 0;
	mess->eth.sourceAddr[2] = 0;
	mess->eth.sourceAddr[3] = 0x01;
	mess->eth.destAddr[0] = 0x7f;
	mess->eth.destAddr[1] = 0;
	mess->eth.destAddr[2] = 0;
	mess->eth.destAddr[3] = 0x01;
	mess->eth.sourcePort[0] = 0x38;
	mess->eth.sourcePort[1] = 0xd6;
	mess->eth.destPort[0] = 0x38;
	mess->eth.destPort[1] = 0xf4;
	mess->eth.checkSum[0] = 0;
	fromInt16be(len - 24, mess->eth.length);
	mess->eth.checkSum[1] = 0;

	// Extract length fo the MAAVLink message
	if (tMess->mav.mav1.mavCode == 0xfe) mavLen = tMess->mav.mav1.payloadLen + 8;
	else if (tMess->mav.mav2.mavCode == 0xfd)	mavLen = tMess->mav.mav2.payloadLen + 12;
	
	// Copy over all of the fields
	for (i = 0; i < mavLen; i++) {
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) mess->mav.mav1.mavCode = tMess->mav.mav1.mavCode;
			else if (i == 1) {
				mess->mav.mav1.payloadLen = tMess->mav.mav1.payloadLen;
				
				payload = (int)mess->mav.mav1.payloadLen;

				if ((mess->mav.mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}				
				
			}
			else if (i == 2) mess->mav.mav1.packetSeq = tMess->mav.mav1.packetSeq;
			else if (i == 3) mess->mav.mav1.systemID = tMess->mav.mav1.systemID;
			else if (i == 4) mess->mav.mav1.compID = tMess->mav.mav1.compID;
			else if (i == 5) mess->mav.mav1.messageID = tMess->mav.mav1.messageID;
			else if (i < 6 + payload) mess->mav.mav1.payload[i - 6] = tMess->mav.mav1.payload[i - 6];
			else mess->mav.mav1.crc[i - 6 - payload] = tMess->mav.mav1.crc[i - 6 - payload];		
		}
		else if (mav2) {
			if (i == 0) {
				mess->mav.mav2.mavCode = tMess->mav.mav2.mavCode;
				mess->mav.mav2.signedMess = tMess->mav.mav2.signedMess;
			}
			else if (i == 1) {
				mess->mav.mav2.payloadLen = tMess->mav.mav2.payloadLen;

				payload = (int)mess->mav.mav2.payloadLen;
				
				if ((mess->mav.mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 2) mess->mav.mav2.incompFlag = tMess->mav.mav2.incompFlag;
			else if (i == 3) mess->mav.mav2.compFlag = tMess->mav.mav2.compFlag;
			else if (i == 4) mess->mav.mav2.packetSeq = tMess->mav.mav2.packetSeq;
			else if (i == 5) mess->mav.mav2.systemID = tMess->mav.mav2.systemID;
			else if (i == 6) mess->mav.mav2.compID = tMess->mav.mav2.compID;
			else if (i < 10) mess->mav.mav2.messageID[i - 7] = tMess->mav.mav2.messageID[i - 7];
			else if (i < 10 + payload) mess->mav.mav2.payload[i - 10] = tMess->mav.mav2.payload[i - 10];
			else if (i < 12 + payload) mess->mav.mav2.crc[i - 10 - payload] = tMess->mav.mav2.crc[i - 10 - payload];
			else mess->mav.mav2.signature[i - 12 - payload] = tMess->mav.mav2.signature[i - 12 - payload];
		}
	}

	return mess;
	
}

/*
 * Convert a MAVLink message to a PCAP message.
 * Inputs: MAVLink message
 * Outputs: PCAP message; NULL if unsuccessful
 */
static pcapMessage_t *mavMessToPcap(mavMessage_t *mMess) {
	// Variable declarations.
	int len, mavLen, i, payload;
	bool mav1, mav2;
	pcapMessage_t *mess;
	struct timeval tv;
	
	// Check arguments.
	if (mMess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	len = 0;
	mav1 = mav2 = false;
	
	// Check type of message.
	if (mMess->mav1.mavCode == 0xfe) {
		mav1 = true;
		len = mMess->mav1.payloadLen + 8;
	}
	else if (mMess->mav2.mavCode == 0xfd) {
		mav2 = true;
		len = mMess->mav2.payloadLen + 12;
		if (mMess->mav2.signedMess) len += 13;
	}
		
	// Allocate space for new MAVlink message.
	if ((mess = (pcapMessage_t *)malloc(sizeof(pcapMessage_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Get the time stamp and insert it.
	gettimeofday(&tv, NULL);
	fromInt32le(tv.tv_sec, mess->prh.timeS);
	fromInt32le(tv.tv_usec, mess->prh.timeUNS);

	// Set length.
	if (mav1) len = mMess->mav1.payloadLen + 40;
	else if (mav2) len = mMess->mav2.payloadLen + 44;

	fromInt32le(len, mess->prh.capturedPacketLength);
	fromInt32le(len, mess->prh.originalPacketLength);
	
	// Fill in ethernet header.
	fromInt32le(2, mess->eth.messageFam);
	mess->eth.iphl = 0x45;
	mess->eth.dsf = 0;
	fromInt16be(len - 4, mess->eth.totalLength);
	mess->eth.id[0] = 0;
	mess->eth.id[1] = 0;
	mess->eth.ffOffset[0] = 0;
	mess->eth.ffOffset[1] = 0;
	mess->eth.ttl = 0x40;
	mess->eth.protocol = 0x11;
	mess->eth.headerChecksum[0] = 0;
	mess->eth.headerChecksum[1] = 0;
	mess->eth.sourceAddr[0] = 0x7f;
	mess->eth.sourceAddr[1] = 0;
	mess->eth.sourceAddr[2] = 0;
	mess->eth.sourceAddr[3] = 0x01;
	mess->eth.destAddr[0] = 0x7f;
	mess->eth.destAddr[1] = 0;
	mess->eth.destAddr[2] = 0;
	mess->eth.destAddr[3] = 0x01;
	mess->eth.sourcePort[0] = 0x38;
	mess->eth.sourcePort[1] = 0xd6;
	mess->eth.destPort[0] = 0x38;
	mess->eth.destPort[1] = 0xf4;
	mess->eth.checkSum[0] = 0;
	fromInt16be(len - 24, mess->eth.length);
	mess->eth.checkSum[1] = 0;

	// Extract length fo the MAVLink message
	if (mMess->mav1.mavCode == 0xfe) mavLen = mMess->mav1.payloadLen + 8;
	else if (mMess->mav2.mavCode == 0xfd)	mavLen = mMess->mav2.payloadLen + 12;
	
	// Copy over all of the fields
	for (i = 0; i < mavLen; i++) {
		// Depending on the messsage type, set the respective fields
		if (mav1) {
			if (i == 0) mess->mav.mav1.mavCode = mMess->mav1.mavCode;
			else if (i == 1) {
				mess->mav.mav1.payloadLen = mMess->mav1.payloadLen;
				
				payload = (int)mess->mav.mav1.payloadLen;

				if ((mess->mav.mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}				
				
			}
			else if (i == 2) mess->mav.mav1.packetSeq = mMess->mav1.packetSeq;
			else if (i == 3) mess->mav.mav1.systemID = mMess->mav1.systemID;
			else if (i == 4) mess->mav.mav1.compID = mMess->mav1.compID;
			else if (i == 5) mess->mav.mav1.messageID = mMess->mav1.messageID;
			else if (i < 6 + payload) mess->mav.mav1.payload[i - 6] = mMess->mav1.payload[i - 6];
			else mess->mav.mav1.crc[i - 6 - payload] = mMess->mav1.crc[i - 6 - payload];		
		}
		else if (mav2) {
			if (i == 0) {
				mess->mav.mav2.mavCode = mMess->mav2.mavCode;
				mess->mav.mav2.signedMess = mMess->mav2.signedMess;
			}
			else if (i == 1) {
				mess->mav.mav2.payloadLen = mMess->mav2.payloadLen;

				payload = (int)mess->mav.mav2.payloadLen;
				
				if ((mess->mav.mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 2) mess->mav.mav2.incompFlag = mMess->mav2.incompFlag;
			else if (i == 3) mess->mav.mav2.compFlag = mMess->mav2.compFlag;
			else if (i == 4) mess->mav.mav2.packetSeq = mMess->mav2.packetSeq;
			else if (i == 5) mess->mav.mav2.systemID = mMess->mav2.systemID;
			else if (i == 6) mess->mav.mav2.compID = mMess->mav2.compID;
			else if (i < 10) mess->mav.mav2.messageID[i - 7] = mMess->mav2.messageID[i - 7];
			else if (i < 10 + payload) mess->mav.mav2.payload[i - 10] = mMess->mav2.payload[i - 10];
			else if (i < 12 + payload) mess->mav.mav2.crc[i - 10 - payload] = mMess->mav2.crc[i - 10 - payload];
			else mess->mav.mav2.signature[i - 12 - payload] = mMess->mav2.signature[i - 12 - payload];
		}
	}

	return mess;
	
}

/*
 * Convert a PCAP message to a MAVLink message.
 * Inputs: PCAP message
 * Outputs: MAVLink message; NULL if unsuccessful
 */
static mavMessage_t *pcapMessToMav(pcapMessage_t *pMess) {
	// Variable declarations.
	mavMessage_t *mess;
	int i, len, payload;
	bool mav1, mav2;

	// Check arguments.
	if (pMess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	len = 0;
	mav1 = mav2 = false;
	
	// Check type of message.
	if (pMess->mav.mav1.mavCode == 0xfe) {
		mav1 = true;
		len = pMess->mav.mav1.payloadLen + 8;
	}
	else if (pMess->mav.mav2.mavCode == 0xfd) {
		mav2 = true;
		len = pMess->mav.mav2.payloadLen + 12;
		if (pMess->mav.mav2.signedMess) len += 13;
	}
		
	// Allocate space for new MAVlink message.
	if ((mess = (mavMessage_t *)malloc(sizeof(mavMessage_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Copy over all of the fields.
	for (i = 0; i < len; i++) {
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) mess->mav1.mavCode = pMess->mav.mav1.mavCode;
			else if (i == 1) {
				mess->mav1.payloadLen = pMess->mav.mav1.payloadLen;
				
				payload = (int)mess->mav1.payloadLen;

				if ((mess->mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}				
				
			}
			else if (i == 2) mess->mav1.packetSeq = pMess->mav.mav1.packetSeq;
			else if (i == 3) mess->mav1.systemID = pMess->mav.mav1.systemID;
			else if (i == 4) mess->mav1.compID = pMess->mav.mav1.compID;
			else if (i == 5) mess->mav1.messageID = pMess->mav.mav1.messageID;
			else if (i < 6 + payload) mess->mav1.payload[i - 6] = pMess->mav.mav1.payload[i - 6];
			else mess->mav1.crc[i - 6 - payload] = pMess->mav.mav1.crc[i - 6 - payload];		
		}
		else if (mav2) {
			if (i == 0) {
				mess->mav2.mavCode = pMess->mav.mav2.mavCode;
				mess->mav2.signedMess = pMess->mav.mav2.signedMess;
			}
			else if (i == 1) {
				mess->mav2.payloadLen = pMess->mav.mav2.payloadLen;

				payload = (int)mess->mav2.payloadLen;
				
				if ((mess->mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 2) mess->mav2.incompFlag = pMess->mav.mav2.incompFlag;
			else if (i == 3) mess->mav2.compFlag = pMess->mav.mav2.compFlag;
			else if (i == 4) mess->mav2.packetSeq = pMess->mav.mav2.packetSeq;
			else if (i == 5) mess->mav2.systemID = pMess->mav.mav2.systemID;
			else if (i == 6) mess->mav2.compID = pMess->mav.mav2.compID;
			else if (i < 10) mess->mav2.messageID[i - 7] = pMess->mav.mav2.messageID[i - 7];
			else if (i < 10 + payload) mess->mav2.payload[i - 10] = pMess->mav.mav2.payload[i - 10];
			else if (i < 12 + payload) mess->mav2.crc[i - 10 - payload] = pMess->mav.mav2.crc[i - 10 - payload];
			else mess->mav2.signature[i - 12 - payload] = pMess->mav.mav2.signature[i - 12 - payload];
		}
	}
	return mess;
}

/*
 * Function to print out PCAP messages in file.
 * Inputs: pointer to message to print
 * Outputs: none
 */
static void printFuncPcap(void *data) {
	// Variable declarations.
	pcapMessage_t *mess;

	// Coerce.
	mess = (pcapMessage_t *)data;

	printMessagePcap(mess);
}

/*
 * Function to print out MAVLink messages in file.
 * Inputs: pointer to message to print
 * Outputs: none
 */
static void printFuncMav(void *data) {
	// Variable declarations.
	mavMessage_t *mess;

	// Coerce.
	mess = (mavMessage_t *)data;

	printMessageMav(mess);
}

/*
 * Function to print out TLOG messages in file.
 * Inputs: pointer to message to print
 * Outputs: none
 */
static void printFuncTlog(void *data) {
	// Variable declarations.
	tlogMessage_t *mess;

	// Coerce.
	mess = (tlogMessage_t *)data;

	printMessageTlog(mess);
}

/*
 * Function to write PCAP messages to a file
 * Inputs: pointer to message to print
 * Outputs: none
 */
static void writeFuncPcap(void *data) {
	// Variable declarations.
	pcapMessage_t *mess;

	// Coerce.
	mess = (pcapMessage_t *)data;

	writeMessageToFilePcap(mess, file);
}

/*
 * Function to write MAVlink message to a file
 * Inputs: pointer to message to print
 * Outputs: none
 */
static void writeFuncMav(void *data) {
	// Variable declarations.
	mavMessage_t *mess;

	// Coerce.
	mess = (mavMessage_t *)data;

	writeMessageToFileMav(mess, file);
}

/*
 * Function to write TLOG message to a file
 * Inputs: pointer to message to print
 * Outputs: none
 */
static void writeFuncTlog(void *data) {
	// Variable declarations.
	tlogMessage_t *mess;

	// Coerce.
	mess = (tlogMessage_t *)data;

	writeMessageToFileTlog(mess, file);
}

/*
 * Free all payload memory for a PCAP message.
 * Inputs: pointer to message to free
 * Outputs: none
 */
static void freePayloadPcap(void *data) {
	// Variable declarations.
	pcapMessage_t *mess;
		
	// Coerce.
	mess = (pcapMessage_t *)data;
	
	if (mess->mav.mav1.mavCode == 0xfe) free(mess->mav.mav1.payload);
	else if (mess->mav.mav2.mavCode == 0xfd) free(mess->mav.mav2.payload);
	
}

/*
 * Free all payload memory for a MAVLink message.
 * Inputs: pointer to message to free
 * Outputs: none
 */
static void freePayloadMav(void *data) {
	// Variable declarations.
	mavMessage_t *mess;
		
	// Coerce.
	mess = (mavMessage_t *)data;
	
	if (mess->mav1.mavCode == 0xfe) free(mess->mav1.payload);
	else if (mess->mav2.mavCode == 0xfd) free(mess->mav2.payload);
	
}

/*
 * Free all payload memory for a TLOG message.
 * Inputs: pointer to message to free
 * Outputs: none
 */
static void freePayloadTlog(void *data) {
	// Variable declarations.
	tlogMessage_t *mess;
		
	// Coerce.
	mess = (tlogMessage_t *)data;
	
	if (mess->mav.mav1.mavCode == 0xfe) free(mess->mav.mav1.payload);
	else if (mess->mav.mav2.mavCode == 0xfd) free(mess->mav.mav2.payload);
	
}

/*
 * Returns a value that is within the enumerated range for a message.
 * Inputs: message ID, filed index
 * Outputs: value in range
 */
static uint8_t fillEnumInRange(int msgID, int ind) {
	// Variable declarations.
	uint8_t val;

	// Except in a few cases, the byte 0x01 will suffice.
	if ((msgID == POSITION_TARGET_GLOBAL_INT && ind == 50) || msgID == SET_MODE)
		val = 0x00;
	else
		val = 0x01;

	return val;
}

/*
 * Writes the actual tests to files based on valid ennumerated values.
 * Inputs: message to write to file, field index, valid enumerated values, maximum length of message, message ID, pointers to pass and fail seeds
 * Outputs: none
 */
static void writeEnumTests(mavMessage_t mess, int ind, vector_t *vec, int maxLen, int msgID, int *passSeed, int *failSeed) {
	// Variable declarations.
	int i;
	char fname[50];
	FILE *fp;
	
	for (i = 0; i <= 255; i++) {
		if (mess.mav1.mavCode == 0xfe) mess.mav1.payload[ind] = (uint8_t)i;
		else if (mess.mav2.mavCode == 0xfd) mess.mav2.payload[ind] = (uint8_t)i;
		
		// If a valid enumrated value, make it a pass test.
		if (vectorContains(vec, compareInt8, (void *)&i)) {
			// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
			sprintf(fname, "pass.%d", (*passSeed)++);
			
			// Open file for writing.
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Error opening file %s for writing.\n", fname);
				return;
			}
			
			// Write message to file.
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
		}
		// Else make it a failure test.
		else {
			// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
			sprintf(fname, "fail.%d", (*failSeed)++);
			
			// Open file for writing.
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Error opening file %s for writing.\n", fname);
				return;
			}
			// Write message to file.
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
		}
	}
}


/*
 * Generates passing and failing tests for enumerated fields
 * Inputs: message ID, field index, message, maximum length of the message, pass and fail seeds
 * Outputs: none
 */
static void generateEnumTests(int msgID, int ind, mavMessage_t mess, int maxLen, int *passSeed, int *failSeed) {
  // Variable declarations.
	vector_t *vec;
	int i;
	uint8_t *ins;

	// Initialise a vector.
	if ((vec = vectorInit()) == NULL) {
		fprintf(stderr, "Error initializing vector.\n");
		return;
	}
	
	// Fill in the valid enumerated values for each message type and field.
	switch(msgID) {
	case HEARTBEAT:
		if (ind == 4) {
			// Fill vector with enumerations
			for (i = 0; i < 45; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 5) {
			// Fill vector with enumerations
			for (i = 0; i < 21; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 7) {
			// Fill vector with enumerations
			for (i = 0; i < 9; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case SET_MODE:
		if (ind == 5) {
			// Fill vector with enumerations
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)64;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)66;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)80;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)88;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)92;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)192;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)194;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)208;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)216;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)220;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case PARAM_VALUE:
		if (ind == 24) {
			// Fill vector with enumerations
			for (i = 1; i <= 10; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case PARAM_SET:
		if (ind == 22) {
			// Fill vector with enumerations
			for (i = 1; i <= 10; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case GPS_RAW_INT:
		if (ind == 28) {
			// Fill vector with enumerations
			for (i = 0; i < 9; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case MISSION_REQUEST:
		if (ind == 4) {
			// Fill vector with enumerations.
			for (i = 0; i <= 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)255;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case MISSION_CURRENT:
		if (ind == 4) {
			// Fill vector with enumerations
			for (i = 0; i < 6; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 5) {
			// Fill vector with enumerations
			for (i = 0; i < 3; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case MISSION_REQUEST_LIST:
		if (ind == 2) {
			// Fill vector with enumerations.
			for (i = 0; i <= 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)255;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case MISSION_COUNT:
		if (ind == 4) {
			// Fill vector with enumerations.
			for (i = 0; i <= 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)255;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case MISSION_CLEAR_ALL:
		if (ind == 2) {
			// Fill vector with enumerations.
			for (i = 0; i <= 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)255;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case MISSION_ACK:
		if (ind == 2) {
			// Fill vector with enumerations.
			for (i = 0; i <= 15; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 3) {
			// Fill vector with enumerations.
			for (i = 0; i <= 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)255;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case MISSION_REQUEST_INT:
		if (ind == 4) {
			// Fill vector with enumerations.
			for (i = 0; i <= 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)255;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case MISSION_ITEM_INT:
		if (ind == 34) {
			// Fill vector with enumerations.
			for (i = 0; i <= 21; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 37) {
			// Fill vector with enumerations.
			for (i = 0; i <= 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)255;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case COMMAND_ACK:
		if (ind == 2) {
			// Fill vector with enumerations
			for (i = 0; i < 10; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case POSITION_TARGET_LOCAL_NED:
		if (ind == 50) {
			// Fill vector with enumerations
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x01;
			if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x07;
			if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x08;
			if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x09;
			if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case POSITION_TARGET_GLOBAL_INT:
		if (ind == 50) {
			// Fill vector with enumerations
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x00;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x03;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
			
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x0A;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x05;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x06;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
			
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x0B;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case DISTANCE_SENSOR:
		if (ind == 10) {
			// Fill vector with enumerations.
			for (i = 0; i <= 4; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
			*ins = (uint8_t)i;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 12) {
			// Fill vector with enumerations.
			for (i = 0; i <= 40; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t )i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = (uint8_t)100;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case BATTERY_STATUS:
		if (ind == 33) {
			// Fill vector with enumerations
			for (i = 0; i < 5; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 34) {
			// Fill vector with enumerations
			for (i = 0; i < 5; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 40) {
			// Fill vector with enumerations
			for (i = 0; i < 8; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 49) {
			// Fill vector with enumerations
			for (i = 0; i < 3; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case EXTENDED_SYS_STATE:
		if (ind == 0) {
			// Fill vector with enumerations
			for (i = 0; i < 5; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 1) {
			// Fill vector with enumerations
			for (i = 0; i < 5; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case STATUSTEXT:
		if (ind == 0) {
			// Fill vector with enumerations
			for (i = 0; i < 8; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case UTM_GLOBAL_POSITION:
		if (ind == 68) {
			// Fill vector with enumerations
			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x01;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x02;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x03;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x10;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");

			if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return;
			}
			*ins = 0x20;
			if (vectorInsertBack(vec, (void *)ins) != 0)
				fprintf(stderr, "Problem inserting value to vector.\n");
		}
		break;
	case OPEN_DRONE_ID_LOCATION:
		if (ind == 52) {
			// Fill vector with enumerations
			for (i = 0; i < 5; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 53) {
			// Fill vector with enumerations
			for (i = 0; i < 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 54) {
			// Fill vector with enumerations
			for (i = 0; i < 13; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 55) {
			// Fill vector with enumerations
			for (i = 0; i < 7; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 56) {
			// Fill vector with enumerations
			for (i = 0; i < 7; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 57) {
			// Fill vector with enumerations
			for (i = 0; i < 5; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 58) {
			// Fill vector with enumerations
			for (i = 0; i < 15; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	case OPEN_DRONE_ID_SYSTEM:
		if (ind == 50) {
			// Fill vector with enumerations
			for (i = 0; i < 3; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		else if (ind == 51) {
			// Fill vector with enumerations
			for (i = 0; i < 2; i++) {
				if ((ins = (uint8_t *)malloc(sizeof(uint8_t))) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return;
				}
				*ins = (uint8_t)i;
				if (vectorInsertBack(vec, (void *)ins) != 0)
					fprintf(stderr, "Problem inserting value to vector.\n");
			}
		}
		break;
	default:
		return;
	}
	
	// Write the tests to pass/fail files.
	writeEnumTests(mess, ind, vec, maxLen, msgID, passSeed, failSeed);
		
	// Clean up memory.
	vectorApply(vec, free);
	vectorFree(vec);
}

/*
 * Given a message ID, initialise the enumerated fields vector and set maximum lengths.
 * Inputs: message id, pointer to MAVLink 1 max length, pointer to MAVLink 2 max length
 * Outputs: enumerated fields vector; NULL if unsuccessfull
 */
static vector_t *initialiseMessageType(int msgID, int *maxLenM1, int *maxLenM2) {
	// Variable declarations.
	vector_t *enFields;
	int *ins;
	
	// Allocate vector for enumerated fields.
	if ((enFields = vectorInit()) == NULL) {
		fprintf(stderr, "Error allocating vector.\n");
		return NULL;
	}
	
	switch (msgID) {
	case HEARTBEAT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 4;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");
		
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 5;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");
		
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 7;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = *maxLenM2 = HEARTBEAT_LEN;
		break;
	case SYS_STATUS:
		*maxLenM1 = SYS_STATUS_LEN - 12;
		*maxLenM2 = SYS_STATUS_LEN;
		break;
	case SYSTEM_TIME:
		*maxLenM1 = *maxLenM2 = SYSTEM_TIME_LEN;
		break;
	case PING:
		*maxLenM1 = *maxLenM2 = PING_LEN;
		break;
	case LINK_NODE_STATUS:
		*maxLenM1 = *maxLenM2 = LINK_NODE_STATUS_LEN;
		break;
	case SET_MODE:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 5;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");
		
		*maxLenM1 = *maxLenM2 = SET_MODE_LEN;
		break;
	case PARAM_REQUEST_READ:
		*maxLenM1 = *maxLenM2 = PARAM_REQUEST_READ_LEN;
		break;
	case PARAM_REQUEST_LIST:
		*maxLenM1 = *maxLenM2 = PARAM_REQUEST_LIST_LEN;
		break;
	case PARAM_VALUE:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 24;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");
		
		*maxLenM1 = *maxLenM2 = PARAM_VALUE_LEN;
		break;
	case PARAM_SET:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 22;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");
		
		*maxLenM1 = *maxLenM2 = PARAM_SET_LEN;
		break;
	case GPS_RAW_INT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 28;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = GPS_RAW_INT_LEN - 22;
		*maxLenM2 = GPS_RAW_INT_LEN;
		break;
	case SCALED_PRESSURE:
		*maxLenM1 = SCALED_PRESSURE_LEN - 2;
		*maxLenM2 = SCALED_PRESSURE_LEN;
		break;
	case ATTITUDE:
		*maxLenM1 = *maxLenM2 = ATTITUDE_LEN;
		break;
	case ATTITUDE_QUATERNION:
		*maxLenM1 = ATTITUDE_QUATERNION_LEN - 16;
		*maxLenM2 = ATTITUDE_QUATERNION_LEN;
		break;
	case LOCAL_POSITION_NED:
		*maxLenM1 = *maxLenM2 = LOCAL_POSITION_NED_LEN;
		break;
	case GLOBAL_POSITION_INT:
		*maxLenM1 = *maxLenM2 = GLOBAL_POSITION_INT_LEN;
		break;
	case MISSION_REQUEST:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 4;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = MISSION_REQUEST_LEN - 1;
		*maxLenM2 = MISSION_REQUEST_LEN;
		break;
	case MISSION_CURRENT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 4;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 5;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = MISSION_CURRENT_LEN - 16;
		*maxLenM2 = MISSION_CURRENT_LEN;
		break;
	case MISSION_REQUEST_LIST:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 2;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = MISSION_REQUEST_LIST_LEN - 1;
		*maxLenM2 = MISSION_REQUEST_LIST_LEN;
		break;
	case MISSION_COUNT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 4;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = MISSION_COUNT_LEN - 5;
		*maxLenM2 = MISSION_COUNT_LEN;
		break;
	case MISSION_CLEAR_ALL:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 2;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = MISSION_CLEAR_ALL_LEN - 1;
		*maxLenM2 = MISSION_CLEAR_ALL_LEN;
		break;
	case MISSION_ITEM_REACHED:
		*maxLenM1 = *maxLenM2 = MISSION_ITEM_REACHED_LEN;
		break;
	case MISSION_ACK:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 2;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 3;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");
 
		*maxLenM1 = MISSION_ACK_LEN - 5;
		*maxLenM2 = MISSION_ACK_LEN;
		break;
	case GPS_GLOBAL_ORIGIN:
		*maxLenM1 = *maxLenM2 = GPS_GLOBAL_ORIGIN_LEN;
		break;
	case MISSION_REQUEST_INT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 4;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = MISSION_REQUEST_INT_LEN - 1;
		*maxLenM2 = MISSION_REQUEST_INT_LEN;
		break;
	case RC_CHANNELS:
		*maxLenM1 = *maxLenM2 = RC_CHANNELS_LEN;;
		break;
	case MANUAL_CONTROL:
		*maxLenM1 = MANUAL_CONTROL_LEN - 19;
		*maxLenM2 = MANUAL_CONTROL_LEN;
		break;
	case MISSION_ITEM_INT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 34;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 37;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = MISSION_ITEM_INT_LEN - 1;
		*maxLenM2 = MISSION_ITEM_INT_LEN;
		break;
	case VFR_HUD:
		*maxLenM1 = *maxLenM2 = VFR_HUD_LEN;
		break;
	case COMMAND_LONG:
		*maxLenM1 = *maxLenM2 = COMMAND_LONG_LEN;
	case COMMAND_ACK:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 2;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = COMMAND_ACK_LEN - 7;
		*maxLenM2 = COMMAND_ACK_LEN;
		break;
	case ATTITUDE_TARGET:
		*maxLenM1 = *maxLenM2 = ATTITUDE_TARGET_LEN;
		break;
	case POSITION_TARGET_LOCAL_NED:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 50;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = *maxLenM2 = POSITION_TARGET_LOCAL_NED_LEN;
		break;
	case POSITION_TARGET_GLOBAL_INT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 50;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = *maxLenM2 = POSITION_TARGET_GLOBAL_INT_LEN;
		break;
	case RADIO_STATUS:
		*maxLenM1 = *maxLenM2 = RADIO_STATUS_LEN;
		break;
	case DISTANCE_SENSOR:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 10;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 12;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = DISTANCE_SENSOR_LEN - 25;
		*maxLenM2 = DISTANCE_SENSOR_LEN;
		break;
	case ALTITUDE:
		*maxLenM1 = *maxLenM2 = ALTITUDE_LEN;
		break;
	case BATTERY_STATUS:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 33;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 34;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 40;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 49;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = BATTERY_STATUS_LEN - 18;
		*maxLenM2 = BATTERY_STATUS_LEN;
		break;
	case AUTOPILOT_VERSION:
		*maxLenM1 = AUTOPILOT_VERSION_LEN - 18;
		*maxLenM2 = AUTOPILOT_VERSION_LEN;
		break;
	case ESTIMATOR_STATUS:
		*maxLenM1 = *maxLenM2 = ESTIMATOR_STATUS_LEN;
		break;
	case VIBRATION:
		*maxLenM1 = *maxLenM2 = VIBRATION_LEN;
		break;
	case HOME_POSITION:
		*maxLenM1 = HOME_POSITION_LEN - 8;
		*maxLenM2 = HOME_POSITION_LEN;
		break;
	case EXTENDED_SYS_STATE:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 0;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 1;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = *maxLenM2 = EXTENDED_SYS_STATE_LEN;
		break;
	case STATUSTEXT:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 0;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM1 = STATUSTEXT_LEN - 3;
		*maxLenM2 = STATUSTEXT_LEN;
		break;
	case PROTOCOL_VERSION:
		*maxLenM2 = PROTOCOL_VERSION_LEN;
		break;
	case UTM_GLOBAL_POSITION:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 68;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM2 = UTM_GLOBAL_POSITION_LEN;
		break;
	case TIME_ESTIMATE_TO_TARGET:
		*maxLenM2 = TIME_ESTIMATE_TO_TARGET_LEN;
		break;
	case COMPONENT_METADATA:
		*maxLenM2 = COMPONENT_METADATA_LEN;
		break;
	case EVENT:
		*maxLenM2 = EVENT_LEN;
		break;
	case CURRENT_EVENT_SEQUENCE:
		*maxLenM2 = CURRENT_EVENT_SEQUENCE_LEN;
		break;
	case OPEN_DRONE_ID_LOCATION:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 52;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 53;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 54;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 55;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 56;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 57;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 58;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM2 = OPEN_DRONE_ID_LOCATION_LEN;
		break;
	case OPEN_DRONE_ID_SYSTEM:
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 50;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return NULL;
		}
		*ins = 51;
		if (vectorInsertBack(enFields, (void *)ins) != 0)
			fprintf(stderr, "Error inserting value to vector.\n");

		*maxLenM2 = OPEN_DRONE_ID_SYSTEM_LEN;
		break;
	default:
		fprintf(stderr, "Invalid message ID %d.\n", msgID);
		return NULL;
	}

	return enFields;
	
}

/*
 * Read a pcap file (header and all messages.
 * Inputs: input file
 * Outputs: pcap file data structure; NULL if unsuccessful
 */
pcap_t *readFilePcap(FILE *ifile) {
	// Variable declarations.
	pPcap_t *pcap;
	int i, payload, bytesRead;
	pcapMessage_t *mess;
	uint8_t buf;
	bool mav1, mav2, signedMess;

	// Check argument.
	if (ifile == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	// Allocate memory and initialise pcap data structure.
	if ((pcap = (pPcap_t *)malloc(sizeof(pPcap_t))) == NULL) {
		fprintf(stderr, "Error allocating memory.\n");
		return NULL;
	}

	// Initialise vector.
	if ((pcap->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Error initialising vector.\n");
		return NULL;
	}

	i = 0;
	
	// Read the first 24 bytes of the file - file header.
	while(i < 24) {
		bytesRead = fread(&buf, sizeof(buf), 1, ifile);

		pcap->header[i] = buf;
				
		i++;
	}

	i = 0;
	payload = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) == 1) {
		// Allocate memory for message.
		if (i == 0) {
			if ((mess = (pcapMessage_t *)malloc(sizeof(pcapMessage_t))) == NULL) {
				fprintf(stderr, "Error allocating memory.\n");
				return NULL;
			}

			mav1 = mav2 = signedMess = false;
		}
		
		if (i < 4) mess->prh.timeS[i] = buf;
		else if (i < 8) mess->prh.timeUNS[i - 4] = buf;
		else if (i < 12) mess->prh.capturedPacketLength[i - 8] = buf;
		else if (i < 16) mess->prh.originalPacketLength[i - 12] = buf;
		else if (i < 20) mess->eth.messageFam[i - 16] = buf;
		else if (i == 20) mess->eth.iphl = buf;
		else if (i == 21) mess->eth.dsf = buf;
		else if (i < 24) mess->eth.totalLength[i - 22] = buf;
		else if (i < 26) mess->eth.id[i - 24] = buf;
		else if (i < 28) mess->eth.ffOffset[i - 26] = buf;
		else if (i == 28) mess->eth.ttl = buf;
		else if (i == 29) mess->eth.protocol = buf;
		else if (i < 32) mess->eth.headerChecksum[i - 30] = buf;
		else if (i < 36) mess->eth.sourceAddr[i - 32] = buf;
		else if (i < 40) mess->eth.destAddr[i - 36] = buf;
		else if (i < 42) mess->eth.sourcePort[i - 40] = buf;
		else if (i < 44) mess->eth.destPort[i - 42] = buf;
		else if (i < 46) mess->eth.length[i - 44] = buf;
		else if (i < 48) mess->eth.checkSum[i - 46] = buf;
		else if (i == 48 && buf == 0xfe) mav1 = true;
		else if (i == 48 && buf == 0xfd) mav2 = true;

		// Depending on the messsage type, set the respective fields.
		if (mav1 && i >= 48) {
			if (i == 48) mess->mav.mav1.mavCode = buf;
			else if (i == 49) {
				mess->mav.mav1.payloadLen = buf;

				payload = (int)buf;
			
				if ((mess->mav.mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 50) mess->mav.mav1.packetSeq = buf;
			else if (i == 51) mess->mav.mav1.systemID = buf;
			else if (i == 52) mess->mav.mav1.compID = buf;
			else if (i == 53) mess->mav.mav1.messageID = buf;		
			else if (i < 54 + payload) mess->mav.mav1.payload[i - 54] = buf;
			else mess->mav.mav1.crc[i - 54 - payload] = buf;		
		}
		else if (mav2 && i >= 48) {
			if (i == 48) mess->mav.mav2.mavCode = buf;
			else if (i == 49) {
				mess->mav.mav2.payloadLen = buf;

				payload = (int)buf;
			
				if ((mess->mav.mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 50) {
				mess->mav.mav2.incompFlag = buf;

				// Detect a signed message.
				if (buf != 0) signedMess = mess->mav.mav2.signedMess = true;
				else mess->mav.mav2.signedMess = false;
			}
			else if (i == 51) mess->mav.mav2.compFlag = buf;
			else if (i == 52) mess->mav.mav2.packetSeq = buf;
			else if (i == 53) mess->mav.mav2.systemID = buf;
			else if (i == 54) mess->mav.mav2.compID = buf;
			else if (i < 58) mess->mav.mav2.messageID[i - 55] = buf;		
			else if (i < 58 + payload) mess->mav.mav2.payload[i - 58] = buf;
			else if (i < 60 + payload) mess->mav.mav2.crc[i - 58 - payload] = buf;
			else mess->mav.mav2.signature[i - 60 - payload] = buf;
		}

		i++;

		// If the end of a message has been reached.
		if (((i == 56 + payload && mav1) || (i == 60 + payload && mav2 && !signedMess) || (i == 73 + payload && mav2 && signedMess)) && payload != 0) {
			// Insert message into vector.
			if (vectorInsertBack(pcap->messages, mess) != 0)
				fprintf(stderr, "Error inserting into vector.\n");
					
			// Reset all variables
			i = 0;
			payload = 0;
		}
	}
	
	return (pcap_t *)pcap;
}

/*
 * Read a MAVlink file (all messages).
 * Inputs: input file
 * Outputs: MAVLink file data structure; NULL if unsuccessful
 */
mavlink_t *readFileMav(FILE *ifile) {
	// Variable declarations.
	pMavlink_t *mav;
	int i, payload, bytesRead;
	mavMessage_t *mess;
	uint8_t buf;
	bool mav1, mav2, signedMess;

	// Check argument.
	if (ifile == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	// Allocate memory and initialise pcap data structure.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Error allocating memory.\n");
		return NULL;
	}

	// Initialise vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Error initialising vector.\n");
		return NULL;
	}

	i = 0;
	payload = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) == 1) {
		// Allocate memory for message.
		if (i == 0) {
			if ((mess = (mavMessage_t *)malloc(sizeof(mavMessage_t))) == NULL) {
				fprintf(stderr, "Error allocating memory.\n");
				return NULL;
			}

			mav1 = mav2 = signedMess = false;
			
			if (buf == 0xfe) mav1 = true;
			else if (buf == 0xfd) mav2 = true;
			
		}
		
		// Depending on the messsage type, set the respective fields.
		if (mav1) {
			if (i == 0) mess->mav1.mavCode = buf;
			else if (i == 1) {
				mess->mav1.payloadLen = buf;

				payload = (int)buf;
			
				if ((mess->mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 2) mess->mav1.packetSeq = buf;
			else if (i == 3) mess->mav1.systemID = buf;
			else if (i == 4) mess->mav1.compID = buf;
			else if (i == 5) mess->mav1.messageID = buf;		
			else if (i < 6 + payload) mess->mav1.payload[i - 6] = buf;
			else mess->mav1.crc[i - 6 - payload] = buf;		
		}
		else if (mav2) {
			if (i == 0) mess->mav2.mavCode = buf;
			else if (i == 1) {
				mess->mav2.payloadLen = buf;

				payload = (int)buf;
			
				if ((mess->mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 2) {
				mess->mav2.incompFlag = buf;

				// Detect is signed message.
				if (buf != 0) signedMess = mess->mav2.signedMess = true;
				else mess->mav2.signedMess = false;
			}
			else if (i == 3) mess->mav2.compFlag = buf;
			else if (i == 4) mess->mav2.packetSeq = buf;
			else if (i == 5) mess->mav2.systemID = buf;
			else if (i == 6) mess->mav2.compID = buf;
			else if (i < 10) mess->mav2.messageID[i - 7] = buf;		
			else if (i < 10 + payload) mess->mav2.payload[i - 10] = buf;
			else if (i < 12 + payload) mess->mav2.crc[i - 10 - payload] = buf;
			else mess->mav2.signature[i - 12 - payload] = buf;
		}

		i++;

		// If the end of a message has been reached.
		if (((i == 8 + payload && mav1) || (i == 12 + payload && mav2 && !signedMess) || (i == 25 + payload && mav2 && signedMess)) && payload != 0) {
			// Insert message into vector.
			if (vectorInsertBack(mav->messages, mess) != 0)
				fprintf(stderr, "Error inserting into vector.\n");
					
			// Reset all variables
			i = 0;
			payload = 0;
		}
	}

	return (mavlink_t *)mav;
}


/*
 * Read a TLOG file (all messages).
 * Inputs: input file
 * Outputs: TLOG file data structure; NULL if unsuccessful
 */
tlog_t *readFileTlog(FILE *ifile) {
	// Variable declarations.
	pTlog_t *tlog;
	int i, payload, bytesRead;
	tlogMessage_t *mess;
	uint8_t buf;
	bool mav1, mav2, signedMess;

	// Check argument.
	if (ifile == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	// Allocate memory and initialise pcap data structure.
	if ((tlog = (pTlog_t *)malloc(sizeof(pTlog_t))) == NULL) {
		fprintf(stderr, "Error allocating memory.\n");
		return NULL;
	}

	// Initialise vector.
	if ((tlog->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Error initialising vector.\n");
		return NULL;
	}

	i = 0;
	payload = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) == 1) {
		// Allocate memory for message.
		if (i == 0) {
			if ((mess = (tlogMessage_t *)malloc(sizeof(tlogMessage_t))) == NULL) {
				fprintf(stderr, "Error allocating memory.\n");
				return NULL;
			}

			mav1 = mav2 = signedMess = false;
		}
		
		if (i < 8) mess->header.bytes[i] = buf;
		else if (i == 8 && buf == 0xfe) mav1 = true;
		else if (i == 8 && buf == 0xfd) mav2 = true;

		// Depending on the messsage type, set the respective fields.
		if (mav1 && i >= 8) {
			if (i == 8) mess->mav.mav1.mavCode = buf;
			else if (i == 9) {
				mess->mav.mav1.payloadLen = buf;

				payload = (int)buf;
			
				if ((mess->mav.mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 10) mess->mav.mav1.packetSeq = buf;
			else if (i == 11) mess->mav.mav1.systemID = buf;
			else if (i == 12) mess->mav.mav1.compID = buf;
			else if (i == 13) mess->mav.mav1.messageID = buf;		
			else if (i < 14 + payload) mess->mav.mav1.payload[i - 14] = buf;
			else mess->mav.mav1.crc[i - 14 - payload] = buf;		
		}
		else if (mav2 && i >= 8) {
			if (i == 8) mess->mav.mav2.mavCode = buf;
			else if (i == 9) {
				mess->mav.mav2.payloadLen = buf;

				payload = (int)buf;
			
				if ((mess->mav.mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL) {
					fprintf(stderr, "Memory allocation failed.\n");
					return NULL;
				}
			}
			else if (i == 10) {
				mess->mav.mav2.incompFlag = buf;

				// Detect if signed message.
				if (buf != 0) signedMess = mess->mav.mav2.signedMess = true;
				else mess->mav.mav2.signedMess = false;
			}
			else if (i == 11) mess->mav.mav2.compFlag = buf;
			else if (i == 12) mess->mav.mav2.packetSeq = buf;
			else if (i == 13) mess->mav.mav2.systemID = buf;
			else if (i == 14) mess->mav.mav2.compID = buf;
			else if (i < 18) mess->mav.mav2.messageID[i - 15] = buf;		
			else if (i < 18 + payload) mess->mav.mav2.payload[i - 18] = buf;
			else if (i < 20 + payload) mess->mav.mav2.crc[i - 18 - payload] = buf;
			else mess->mav.mav2.signature[i - 20 - payload] = buf;
		}

		i++;

		// If the end of a message has been reached.
		if (((i == 16 + payload && mav1) || (i == 20 + payload && mav2 && !signedMess) || (i == 33 + payload && mav2 && signedMess)) && payload != 0) {
			// Insert message into vector.
			if (vectorInsertBack(tlog->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting into vector.\n");
					
			// Reset all variables
			i = 0;
			payload = 0;
		}
	}

	return (tlog_t *)tlog;
}

/*
 * Print out the entire contents of a stored PCAP file.
 * Inputs: PCAP file
 * Outputs: none
 */
void printFilePcap(pcap_t *pcapP) {	
	// Variable declarations.
	int i;
	pPcap_t *pcap;

	// Check argument.
	if (pcapP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;
	
	// First print header.
	printf("Header: ");

	for (i = 0; i < 24; i++)
		printf("%02x ", pcap->header[i]);

	printf("\n");
				 
	// Now print each message.
	vectorApply(pcap->messages, printFuncPcap);
	
}

/*
 * Print out the entire contents of a stored MAVlink file.
 * Inputs: MAVLink file
 * Outputs: none
 */
void printFileMav(mavlink_t *mavP) {
	// Variable declarations.
	pMavlink_t *mav;
	
	// Check argument.
	if (mavP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	mav = (pMavlink_t *)mavP;
	
	// Now print each message.
	vectorApply(mav->messages, printFuncMav);
		
}

/*
 * Print out the entire contents of a stored TLOG file.
 * Inputs: TLOG file
 * Outputs: none
 */
void printFileTlog(tlog_t *tlogP) {
	// Variable declarations.
	pTlog_t *tlog;
	
	// Check argument.
	if (tlogP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;
	
	// Now print each message.
	vectorApply(tlog->messages, printFuncTlog);
		
}

/*
 * Write the entire contents of a PCAP file structure to a file.
 * Inputs: PCAP file, file to write to
 * Outputs: none
 */
void writeToFilePcap(pcap_t *pcapP, FILE *fp) {
	// Variable declarations.
	int i;
	uint8_t buf;
	pPcap_t *pcap;
	
	// Check argument.
	if (pcapP == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;

	// Write PCAP file header.
	for (i = 0; i < 24; i++) {
		buf = pcap->header[i];
		
		if (fwrite(&buf, sizeof(buf), 1, fp) != 1)
			fprintf(stderr, "Error writing byte %02x to file.\n", buf);
	}

	// Set global variable to keeptrack of file.
	file = fp;
	
	// Now print each message.
	vectorApply(pcap->messages, writeFuncPcap);

}

/*
 * Write the entire contents of a MAVLink file structure to a file.
 * Inputs: MAVLink file, file to write to
 * Outputs: none
 */
void writeToFileMav(mavlink_t *mavP, FILE *fp) {
	// Variable declarations.
	pMavlink_t *mav;
	
	// Check argument.
	if (mavP == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}

	// Coerce.
	mav = (pMavlink_t *)mavP;
	
	// Set global variable to keeptrack of file.
	file = fp;

	// Now print each message.
	vectorApply(mav->messages, writeFuncMav);
}

/*
 * Write the entire contents of a TLOG file structure to a file.
 * Inputs: TLOG file, file to write to
 * Outputs: none
 */
void writeToFileTlog(tlog_t *tlogP, FILE *fp) {
	// Variable declarations.
	pTlog_t *tlog;
	
	// Check argument.
	if (tlogP == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;
	
	// Set global variable to keeptrack of file.
	file = fp;
	
	// Now print each message.
	vectorApply(tlog->messages, writeFuncTlog);

}

/*
 * Frees a PCAP file structure and all messages associated with it.
 * Inputs: PCAP file
 * Outputs: none
 */
void freePcap(pcap_t *pcapP) {
	// Variable declarations.
	pPcap_t *pcap;

	// Check arguments.
	if (pcapP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;

	// Free payloads of each message.
	vectorApply(pcap->messages, freePayloadPcap);

	// Free each message.
	vectorApply(pcap->messages, free);
	
	// Deallocate memory from the vector.
	vectorFree(pcap->messages);
	
	// Free the pcap structure itself.
	free(pcap);
	
}

/*
 * Frees a MAVLink file structure and all messages associated with it.
 * Inputs: MAVLink file
 * Outputs: none
 */
void freeMav(mavlink_t *mavP)  {
	// Variable declarations.
	pMavlink_t *mav;

	// Check arguments.
	if (mavP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	mav = (pMavlink_t *)mavP;
	
	// Free payloads of each message.
	vectorApply(mav->messages, freePayloadMav);

	// Free each message.
	vectorApply(mav->messages, free);
	
	// Deallocate memory from the vector.
	vectorFree(mav->messages);
	
	// Free the pcap structure itself.
	free(mav);
	
}

/*
 * Frees a TLOG file structure and all messages associated with it.
 * Inputs: TLOG file
 * Outputs: none
 */
void freeTlog(tlog_t *tlogP) {
	// Variable declarations.
	pTlog_t *tlog;

	// Check arguments.
	if (tlogP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;
	
	// Free payloads of each message.
	vectorApply(tlog->messages, freePayloadTlog);

	// Free each message.
	vectorApply(tlog->messages, free);
	
	// Deallocate memory from the vector.
	vectorFree(tlog->messages);
	
	// Free the pcap structure itself.
	free(tlog);
	
}

/*
 * Convert a TLOG file structure to a MAVLink file structure.
 * Inputs: TLOG file structure
 * Outputs: MAVLink file structure; NUKLL if unsuccessful
 */
mavlink_t *tlogToMav(tlog_t *tlogP) {
	// Variable declarations.
	pTlog_t *tlog;
	pMavlink_t *mav;
	mavMessage_t *mess;
	tlogMessage_t *tMess;
	int i;
	
	// Check arguments.
	if (tlogP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;

	// Allcoate memory for MAVLink file structure.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Extract the MAVLink portion of each message.
	for (i = 0; i < vectorGetSize(tlog->messages); i++) {
		// Extract message from vector.
		if ((tMess = (tlogMessage_t *)vectorGetElement(tlog->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message %d from vector.\n", i);

		if ((mess = tlogMessToMav(tMess)) == NULL)
			fprintf(stderr, "Error converting message %d.\n", i);

		// Insert into vector.
		if (vectorInsertBack(mav->messages, (void *)mess) != 0)
			fprintf(stderr, "Insertion into vector failed.\n");
		
	}

	return (mavlink_t *)mav;
}

/*
 * Convert a TLOG file structure to a PCAP file structure.
 * Inputs: TLOG file structure
 * Outputs: PCAP file structure; NUKLL if unsuccessful
 */
pcap_t *tlogToPcap(tlog_t *tlogP) {
	// Variable declarations.
	pTlog_t *tlog;
	pPcap_t *pcap;
	pcapMessage_t *pMess;
	tlogMessage_t *tMess;
	int i;

	// Check arguments.
	if (tlogP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;

	// Allocate space for pacp file structure.
	if ((pcap = (pPcap_t *)malloc(sizeof(pPcap_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise a vector to jhold messages.
	if ((pcap->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}
	
	// Fill the file header.
	pcap->header[0] = 0xd4;
	pcap->header[1] = 0xc3; 
	pcap->header[2] = 0xb2;
	pcap->header[3] = 0xa1;
	pcap->header[4] = 0x02;
	pcap->header[5] = 0;
	pcap->header[6] = 0x04;
	pcap->header[7] = 0;
	for (i = 8; i < 24; i++)
		pcap->header[i] = 0;

	// For each message in the TLOG file create a corresponding PCAP message.
	for (i = 0; i < vectorGetSize(tlog->messages); i++) {
		// Extract message from vector.
		if ((tMess = (tlogMessage_t *)vectorGetElement(tlog->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message %d from vector.\n", i);
		
		if ((pMess = tlogMessToPcap(tMess)) == NULL)
			fprintf(stderr, "Error converting message %d.\n", i);

		// Insert into vector.
		if (vectorInsertBack(pcap->messages, (void *)pMess) != 0)
			fprintf(stderr, "Insertion into vector failed.\n");
	}
	
	return (pcap_t *)pcap;
	
}

/*
 * Convert a MAVLink file structure to a PCAP file structure.
 * Inputs: MAVLink file structure
 * Outputs: PCAP file structure; NUKLL if unsuccessful
 */
pcap_t *mavToPcap(mavlink_t *mavP) {
	// Variable declarations.
	pMavlink_t *mav;
	pPcap_t *pcap;
	pcapMessage_t *pMess;
	mavMessage_t *mMess;
	int i;

	// Check arguments.
	if (mavP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	// Coerce.
	mav = (pMavlink_t *)mavP;

	// Allocate space for pacp file structure.
	if ((pcap = (pPcap_t *)malloc(sizeof(pPcap_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise a vector to jhold messages.
	if ((pcap->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}
	
	// Fill the file header.
	pcap->header[0] = 0xd4;
	pcap->header[1] = 0xc3; 
	pcap->header[2] = 0xb2;
	pcap->header[3] = 0xa1;
	pcap->header[4] = 0x02;
	pcap->header[5] = 0;
	pcap->header[6] = 0x04;
	pcap->header[7] = 0;
	for (i = 8; i < 24; i++)
		pcap->header[i] = 0;
	
	// For each message in the MAVLink file create a corresponding PCAP message.
	for (i = 0; i < vectorGetSize(mav->messages); i++) {
		// Extract message from vector.
		if ((mMess = (mavMessage_t *)vectorGetElement(mav->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message %d from vector.\n", i);
		
		if ((pMess = mavMessToPcap(mMess)) == NULL)
			fprintf(stderr, "Error converting message %d.\n", i);

		// Insert into vector.
		if (vectorInsertBack(pcap->messages, (void *)pMess) != 0)
			fprintf(stderr, "Insertion into vector failed.\n");
	}
	
	return (pcap_t *)pcap;

}

/*
 * Convert a PCAP file structure to a MAVLink file structure.
 * Inputs: PCAP file structure
 * Outputs: MAVLink file structure; NUKLL if unsuccessful
 */
mavlink_t *pcapToMav(pcap_t *pcapP) {
	// Variable declarations.
	pPcap_t *pcap;
	pMavlink_t *mav;
	mavMessage_t *mess;
	pcapMessage_t *pMess;
	int i;
	
	// Check arguments.
	if (pcapP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;

	// Allcoate memory for MAVLink file structure.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Extract the MAVLink portion of each message.
	for (i = 0; i < vectorGetSize(pcap->messages); i++) {
		// Extract message from vector.
		if ((pMess = (pcapMessage_t *)vectorGetElement(pcap->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message %d from vector.\n", i);

		if ((mess = pcapMessToMav(pMess)) == NULL)
			fprintf(stderr, "Error converting message %d.\n", i);

		// Insert into vector.
		if (vectorInsertBack(mav->messages, (void *)mess) != 0)
			fprintf(stderr, "Insertion into vector failed.\n");

	}

	return (mavlink_t *)mav;
}

/*
 * Count the number of unique message IDs in a PCAP file.
 * Inputs: PCAP file structure
 * Outputs: none
 */
void countIDSPcap(pcap_t *pcapP) {
	// Variable declarations.
	pPcap_t *pcap;
	vector_t *ids;
	int i, id, *idp;
	pcapMessage_t *mess;
	
	// Check arguments.
	if (pcapP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;
	
	// Initialise vector
	if ((ids = vectorInit()) == NULL) {
		fprintf(stderr, "Error initialising vector.\n");
		return;
	}

	// Insert unique ids into vector.
	for (i = 0; i < vectorGetSize(pcap->messages); i++) {
		// Get the relevant message.
		if ((mess = (pcapMessage_t *)vectorGetElement(pcap->messages, i)) == NULL)
			fprintf(stderr, "Error getting element %d.\n", i);

		if (mess->mav.mav1.mavCode == 0xfe)
			id = (int)mess->mav.mav1.messageID;
		else if (mess->mav.mav2.mavCode == 0xfd)
			id = toInt24le(mess->mav.mav2.messageID);

		// If not in vector insert.
		if (!(vectorContains(ids, compareInt, (void *)&id))) {
			// Allocate memry for id.
			if ((idp = (int *)malloc(sizeof(int))) == NULL)
				fprintf(stderr, "Memory allocation failed.\n");

			*idp = id;

			if (vectorInsertBack(ids, (void *)idp) != 0)
				fprintf(stderr, "Error inserting id %d into vector.\n", *idp);
		}
	}
	
	printf("There are %d unique message IDs:\n", vectorGetSize(ids));
	vectorApply(ids, printID);

	// Clean up memory.
	vectorApply(ids, free);
	vectorFree(ids);
}

/*
 * Count the number of unique message IDs in a MAVLink file.
 * Inputs: MAVLink file structure
 * Outputs: none
 */
void countIDSMav(mavlink_t *mavP) {
	// Variable declarations.
	pMavlink_t *mav;
	vector_t *ids;
	int i, id, *idp;
	mavMessage_t *mess;
	
	// Check arguments.
	if (mavP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	mav = (pMavlink_t *)mavP;
	
	// Initialise vector
	if ((ids = vectorInit()) == NULL) {
		fprintf(stderr, "Error initialising vector.\n");
		return;
	}

	// Insert unique ids into vector.
	for (i = 0; i < vectorGetSize(mav->messages); i++) {
		// Get the relevant message.
		if ((mess = (mavMessage_t *)vectorGetElement(mav->messages, i)) == NULL)
			fprintf(stderr, "Error getting element %d.\n", i);

		if (mess->mav1.mavCode == 0xfe)
			id = (int)mess->mav1.messageID;
		else if (mess->mav2.mavCode == 0xfd)
			id = toInt24le(mess->mav2.messageID);

		// If not in vector insert.
		if (!(vectorContains(ids, compareInt, (void *)&id))) {
			// Allocate memry for id.
			if ((idp = (int *)malloc(sizeof(int))) == NULL)
				fprintf(stderr, "Memory allocation failed.\n");

			*idp = id;

			if (vectorInsertBack(ids, (void *)idp) != 0)
				fprintf(stderr, "Error inserting id %d into vector.\n", *idp);
		}
	}
	
	printf("There are %d unique message IDs:\n", vectorGetSize(ids));
	vectorApply(ids, printID);

	// Clean up memory.
	vectorApply(ids, free);
	vectorFree(ids);
}

/*
 * Count the number of unique message IDs in a TLOG file.
 * Inputs: TLOG file structure
 * Outputs: none
 */
void countIDSTlog(tlog_t *tlogP) {
	// Variable declarations.
	pTlog_t *tlog;
	vector_t *ids;
	int i, id, *idp;
	tlogMessage_t *mess;
	
	// Check arguments.
	if (tlogP == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;
	
	// Initialise vector
	if ((ids = vectorInit()) == NULL) {
		fprintf(stderr, "Error initialising vector.\n");
		return;
	}

	// Insert unique ids into vector.
	for (i = 0; i < vectorGetSize(tlog->messages); i++) {
		// Get the relevant message.
		if ((mess = (tlogMessage_t *)vectorGetElement(tlog->messages, i)) == NULL)
			fprintf(stderr, "Error getting element %d.\n", i);

		if (mess->mav.mav1.mavCode == 0xfe)
			id = (int)mess->mav.mav1.messageID;
		else if (mess->mav.mav2.mavCode == 0xfd)
			id = toInt24le(mess->mav.mav2.messageID);

		// If not in vector insert.
		if (!(vectorContains(ids, compareInt, (void *)&id))) {
			// Allocate memry for id.
			if ((idp = (int *)malloc(sizeof(int))) == NULL)
				fprintf(stderr, "Memory allocation failed.\n");

			*idp = id;

			if (vectorInsertBack(ids, (void *)idp) != 0)
				fprintf(stderr, "Error inserting id %d into vector.\n", *idp);
		}
	}
	
	printf("There are %d unique message IDs:\n", vectorGetSize(ids));
	vectorApply(ids, printID);

	// Clean up memory.
	vectorApply(ids, free);
	vectorFree(ids);
}

/*
 * Extract all messages of a given ID(s) from a PCAP file and place in a MAVLink file.
 * Inputs: PCAP file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByIdPcapToMav(pcap_t *pcapP, vector_t *ids) {
	// Variable declarations.
	pPcap_t *pcap;
	pMavlink_t *mav;
	mavMessage_t *mess;
	pcapMessage_t *pMess;
	int i, id;
	
	// Check arguments.
	if (pcapP == NULL || ids == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Check if each message's ID is in the vector
	for (i = 0; i < vectorGetSize(pcap->messages); i++) {
		// Extract message.
		if ((pMess = (pcapMessage_t *)vectorGetElement(pcap->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Get id.
		if (pMess->mav.mav1.mavCode == 0xfe)
			id = (int)pMess->mav.mav1.messageID;
		else if (pMess->mav.mav2.mavCode == 0xfd)
			id = toInt24le(pMess->mav.mav2.messageID);
		
		// Check if ID is to be extracted.
		if (vectorContains(ids, compareInt, (void *)&id)) {
			// Extract message and insert into MAVLink file.
			if ((mess = pcapMessToMav(pMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");
		}
	}
	return (mavlink_t *)mav;
}

/*
 * Extract all messages of a given ID(s) from a TLOG file and place in a MAVLink file.
 * Inputs: TLOG file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByIdTlogToMav(tlog_t *tlogP, vector_t *ids) {
	// Variable declarations.
	pTlog_t *tlog;
	pMavlink_t *mav;
	mavMessage_t *mess;
	tlogMessage_t *tMess;
	int i, id;
	
	// Check arguments.
	if (tlogP == NULL || ids == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Check if each message's ID is in the vector
	for (i = 0; i < vectorGetSize(tlog->messages); i++) {
		// Extract message.
		if ((tMess = (tlogMessage_t *)vectorGetElement(tlog->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Get id.
		if (tMess->mav.mav1.mavCode == 0xfe)
			id = (int)tMess->mav.mav1.messageID;
		else if (tMess->mav.mav2.mavCode == 0xfd)
			id = toInt24le(tMess->mav.mav2.messageID);
		
		// Check if ID is to be extracted.
		if (vectorContains(ids, compareInt, (void *)&id)) {
			// Extract message and insert into MAVLink file.
			if ((mess = tlogMessToMav(tMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");
		}
	}
	return (mavlink_t *)mav;
}

/*
 * Extract all messages of a given ID(s) from a MAVLink file and place in a MAVLink file.
 * Inputs: MAVLink file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByIdMavToMav(mavlink_t *mavP, vector_t *ids) {
	// Variable declarations.
	pMavlink_t *m, *mav;
	mavMessage_t *mMess, *mess;
	int i, id;
	
	// Check arguments.
	if (mavP == NULL || ids == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	m = (pMavlink_t *)mavP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Check if each message's ID is in the vector
	for (i = 0; i < vectorGetSize(m->messages); i++) {
		// Extract message.
		if ((mMess = (mavMessage_t *)vectorGetElement(m->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Get id.
		if (mMess->mav1.mavCode == 0xfe)
			id = (int)mMess->mav1.messageID;
		else if (mMess->mav2.mavCode == 0xfd)
			id = toInt24le(mMess->mav2.messageID);
		
		// Check if ID is to be extracted.
		if (vectorContains(ids, compareInt, (void *)&id)) {
			// Extract message and insert into MAVLink file.
			if ((mess = mavMessToMav(mMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");
		}
	}
	return (mavlink_t *)mav;
}

/*
 * Extract a single message of a given ID(s) from a PCAP file and place in a MAVLink file.
 * Inputs: PCAP file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractOneByIdPcapToMav(pcap_t *pcapP, vector_t *ids) {
	// Variable declarations.
	pPcap_t *pcap;
	pMavlink_t *mav;
	mavMessage_t *mess;
	pcapMessage_t *pMess;
	int i, id;
	bool found;
	
	// Check arguments.
	if (pcapP == NULL || ids == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	found = false;
	
	// Check if each message's ID is in the vector
	for (i = 0; i < vectorGetSize(pcap->messages) && !found; i++) {
		// Extract message.
		if ((pMess = (pcapMessage_t *)vectorGetElement(pcap->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Get id.
		if (pMess->mav.mav1.mavCode == 0xfe)
			id = (int)pMess->mav.mav1.messageID;
		else if (pMess->mav.mav2.mavCode == 0xfd)
			id = toInt24le(pMess->mav.mav2.messageID);
		
		// Check if ID is to be extracted.
		if (vectorContains(ids, compareInt, (void *)&id)) {
			// Extract message and insert into MAVLink file.
			if ((mess = pcapMessToMav(pMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");

			// Message has been found.
			found = true;
		}
	}
	return (mavlink_t *)mav;
}

/*
 * Extract a single message of a given ID(s) from a TLOG file and place in a MAVLink file.
 * Inputs: TLOG file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractOneByIdTlogToMav(tlog_t *tlogP, vector_t *ids) {
	// Variable declarations.
	pTlog_t *tlog;
	pMavlink_t *mav;
	mavMessage_t *mess;
	tlogMessage_t *tMess;
	int i, id;
	bool found;
	
	// Check arguments.
	if (tlogP == NULL || ids == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	found = false;
	
	// Check if each message's ID is in the vector
	for (i = 0; i < vectorGetSize(tlog->messages) && !found; i++) {
		// Extract message.
		if ((tMess = (tlogMessage_t *)vectorGetElement(tlog->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Get id.
		if (tMess->mav.mav1.mavCode == 0xfe)
			id = (int)tMess->mav.mav1.messageID;
		else if (tMess->mav.mav2.mavCode == 0xfd)
			id = toInt24le(tMess->mav.mav2.messageID);
		
		// Check if ID is to be extracted.
		if (vectorContains(ids, compareInt, (void *)&id)) {
			// Extract message and insert into MAVLink file.
			if ((mess = tlogMessToMav(tMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");

			// Message has been found.
			found = true;
		}
	}
	return (mavlink_t *)mav;
}

/*
 * Extract a single message of a given ID(s) from a MAVLink file and place in a MAVLink file.
 * Inputs: MAVLink file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractOneByIdMavToMav(mavlink_t *mavP, vector_t *ids) {
	// Variable declarations.
	pMavlink_t *m, *mav;
	mavMessage_t *mMess, *mess;
	int i, id;
	bool found;
	
	// Check arguments.
	if (mavP == NULL || ids == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	m = (pMavlink_t *)mavP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	found = false;
	
	// Check if each message's ID is in the vector
	for (i = 0; i < vectorGetSize(m->messages) && !found; i++) {
		// Extract message.
		if ((mMess = (mavMessage_t *)vectorGetElement(m->messages, i)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Get id.
		if (mMess->mav1.mavCode == 0xfe)
			id = (int)mMess->mav1.messageID;
		else if (mMess->mav2.mavCode == 0xfd)
			id = toInt24le(mMess->mav2.messageID);
		
		// Check if ID is to be extracted.
		if (vectorContains(ids, compareInt, (void *)&id)) {
			// Extract message and insert into MAVLink file.
			if ((mess = mavMessToMav(mMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");

			// Message has been found.
			found = true;
		}
	}
	return (mavlink_t *)mav;
}


/*
 * Extract messages of a given number in the message sequence (indexed base 1).
 * Inputs: PCAP file, vector of message numbers
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByNumberPcapToMav(pcap_t *pcapP, vector_t *numbers) {
	// Variable declarations.
	pPcap_t *pcap;
	pMavlink_t *mav;
	mavMessage_t *mess;
	pcapMessage_t *pMess;
	int i;
	
	// Check arguments.
	if (pcapP == NULL || numbers == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	pcap = (pPcap_t *)pcapP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Check if each message's ID is in the vector
	for (i = 1; i <= vectorGetSize(pcap->messages); i++) {
		// Extract message.
		if ((pMess = (pcapMessage_t *)vectorGetElement(pcap->messages, i - 1)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Check if number is to be extracted.
		if (vectorContains(numbers, compareInt, (void *)&i)) {
			// Extract message and insert into MAVLink file.
			if ((mess = pcapMessToMav(pMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");
		}
	}
	return (mavlink_t *)mav;
}

/*
 * Extract messages of a given number in the message sequence (indexed base 1).
 * Inputs: TLOG file, vector of message numbers
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByNumberTlogToMav(tlog_t *tlogP, vector_t *numbers) {
	// Variable declarations.
	pTlog_t *tlog;
	pMavlink_t *mav;
	mavMessage_t *mess;
	tlogMessage_t *tMess;
	int i;
	
	// Check arguments.
	if (tlogP == NULL || numbers == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	tlog = (pTlog_t *)tlogP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Check if each message's ID is in the vector
	for (i = 1; i <= vectorGetSize(tlog->messages); i++) {
		// Extract message.
		if ((tMess = (tlogMessage_t *)vectorGetElement(tlog->messages, i - 1)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Check if number is to be extracted.
		if (vectorContains(numbers, compareInt, (void *)&i)) {
			// Extract message and insert into MAVLink file.
			if ((mess = tlogMessToMav(tMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");
		}
	}
	return (mavlink_t *)mav;
}

/*
 * Extract messages of a given number in the message sequence (indexed base 1).
 * Inputs: MAVLink file, vector of message numbers
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByNumberMavToMav(mavlink_t *mavP, vector_t *numbers) {
	// Variable declarations.
	pMavlink_t *m, *mav;
	mavMessage_t *mMess, *mess;
	int i;
	
	// Check arguments.
	if (mavP == NULL || numbers == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return NULL;
	}

	// Coerce.
	m = (pMavlink_t *)mavP;

	// Allocate memory for MAVLink file.
	if ((mav = (pMavlink_t *)malloc(sizeof(pMavlink_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise message vector.
	if ((mav->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		return NULL;
	}

	// Check if each message's ID is in the vector
	for (i = 1; i <= vectorGetSize(m->messages); i++) {
		// Extract message.
		if ((mMess = (mavMessage_t *)vectorGetElement(m->messages, i - 1)) == NULL)
			fprintf(stderr, "Error extracting message from vector.\n");

		// Check if number is to be extracted.
		if (vectorContains(numbers, compareInt, (void *)&i)) {
			// Extract message and insert into MAVLink file.
			if ((mess = mavMessToMav(mMess)) == NULL)
				fprintf(stderr, "Error extracting message.\n");

			// Add message to MAVLink vector.
			if (vectorInsertBack(mav->messages, (void *)mess) != 0)
				fprintf(stderr, "Error inserting message into vector.\n");
		}
	}
	return (mavlink_t *)mav;
}

/* Generate a full suite of tests for a given messageID.
 * Inputs: message ID, initial pass test number, initial fail test number
 * Outputs: none
 */
void generateTests(int msgID, int passSeed, int failSeed) {
	// Variable declarations.
	int maxLenM1, maxLenM2, i, j;
	FILE *fp;
	char fname[50];
	vector_t *enFields;
	mavMessage_t mess;
	uint8_t randomByte, old;
	
	// Seed a random number generator.
	srand(time(NULL));

	// Initialise the given message type.
	if ((enFields = initialiseMessageType(msgID, &maxLenM1, &maxLenM2)) == NULL) {
		fprintf(stderr, "Error initialising message type.\n");
		return;
	}
	
	// Only test MAVLink 1 messages for message IDs under 255.
	if (msgID <= 255) {
		// Create a message with random byte values in wildcard fields and all other fields fixed.
		mess.mav1.mavCode = 0xfe;
		mess.mav1.payloadLen = (uint8_t)maxLenM1;
		mess.mav1.packetSeq = (uint8_t)(rand() % 256);
		mess.mav1.systemID = (uint8_t)(rand() % 256);
		mess.mav1.compID = (uint8_t)(rand() % 256);
		mess.mav1.messageID = (uint8_t)msgID;
		
		// Allocate memory for the payload.
		if ((mess.mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*maxLenM1)) == NULL)
			fprintf(stderr, "Memory allocation failed.\n");

		// Fill payload bytes.
		for (i = 0; i < maxLenM1; i++) {
			// Generate a random byte for wildcard fields.
			randomByte = (uint8_t)(rand() % 256);

			// Differentiate between ennumerated and non-ennumerated fields.
			if (!(vectorContains(enFields, compareInt, (void *)&i))) mess.mav1.payload[i] = randomByte;
			else mess.mav1.payload[i] = fillEnumInRange(msgID, i);
		}
		
		// Fill in the crc with random bytes.
		mess.mav1.crc[0] = (uint8_t)(rand() % 256);
		mess.mav1.crc[1] = (uint8_t)(rand() % 256);

		// Generate a single passing test.
		sprintf(fname, "pass.%d", passSeed++);
				
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}

		writeMessageToFileMav(&mess, fp);

		fclose(fp);
		
		// Generate failing tests for MAVLink code.
		old = mess.mav1.mavCode;
		for (i = 0; i <= 255; i++) {
			mess.mav1.mavCode = (uint8_t)i;
			if (i != old) {
				// Generate a single passing test.
				sprintf(fname, "fail.%d", failSeed++);
				
				if ((fp = fopen(fname, "wb")) == NULL) {
					fprintf(stderr, "Failed to open file.\n");
					return;
				}

				writeAltFormatMessageToFileMav(&mess, fp, 1, mess.mav1.payloadLen, mess.mav1.payloadLen + 8);
				
				fclose(fp);
				
			}
		}
		mess.mav1.mavCode = old;

		// Generate failing tests for payload length.
		old = mess.mav1.payloadLen;
		for (i = 0; i <= 255; i++) {
			mess.mav1.payloadLen = (uint8_t)i;
			if (i != old) {
				// Generate a single failing test.
				sprintf(fname, "fail.%d", failSeed++);
			
				if ((fp = fopen(fname, "wb")) == NULL) {
					fprintf(stderr, "Failed to open file.\n");
					return;
				}

				writeAltFormatMessageToFileMav(&mess, fp, 1, old, old + 8);
				
				fclose(fp);
				
			}
		}
		mess.mav1.payloadLen = old;

		// Generate passing tests for packet sequence.
		old = mess.mav1.packetSeq;
		for (i = 0; i <= 255; i++) {
			mess.mav1.packetSeq = (uint8_t)i;
			if (i != old) {
				// Generate a single passing test.
				sprintf(fname, "pass.%d", passSeed++);
			
				if ((fp = fopen(fname, "wb")) == NULL) {
					fprintf(stderr, "Failed to open file.\n");
					return;
				}
				
				writeMessageToFileMav(&mess, fp);
				
				fclose(fp);
				
			}
		}
		mess.mav1.packetSeq = old;

		// Generate passing tests for system ID.
		old = mess.mav1.systemID;
		for (i = 0; i <= 255; i++) {
			mess.mav1.systemID = (uint8_t)i;
			if (i != old) {
				// Generate a single passing test.
				sprintf(fname, "pass.%d", passSeed++);
			
				if ((fp = fopen(fname, "wb")) == NULL) {
					fprintf(stderr, "Failed to open file.\n");
					return;
				}
				
				writeMessageToFileMav(&mess, fp);
				
				fclose(fp);
				
			}
		}
		mess.mav1.systemID = old;

		// Generate passing tests for component ID.
		old = mess.mav1.compID;
		for (i = 0; i <= 255; i++) {
			mess.mav1.compID = (uint8_t)i;
			if (i != old) {
				// Generate a single passing test.
				sprintf(fname, "pass.%d", passSeed++);
			
				if ((fp = fopen(fname, "wb")) == NULL) {
					fprintf(stderr, "Failed to open file.\n");
					return;
				}
				
				writeMessageToFileMav(&mess, fp);
				
				fclose(fp);
				
			}
		}
		mess.mav1.compID = old;

		// Generate passing and failing tests of all payload fields.
		for (i = 0; i < maxLenM1; i++) {
			old = mess.mav1.payload[i];
			// First deal with non-enumerated fields.
			if (!(vectorContains(enFields, compareInt, (void *)&i))) {
				for (j = 0; j < 255; j++) {
					mess.mav1.payload[i] = j;
					if (j != old) {
						// Generate a single passing test.
						sprintf(fname, "pass.%d", passSeed++);
						
						if ((fp = fopen(fname, "wb")) == NULL) {
							fprintf(stderr, "Failed to open file.\n");
							return;
						}
						
						writeMessageToFileMav(&mess, fp);
						
						fclose(fp);
						
					}
				}
			}
			else {
				// Generate tests for enumerated fields.
				generateEnumTests(msgID, i, mess, maxLenM1, &passSeed, &failSeed);
			}	
			mess.mav1.payload[i] = old;
		}
		
		// Generate passing tests for all fields of CRC.
		for (i = 0; i < 2; i++) {
			old = mess.mav1.crc[i];
			for (j = 0; j < 255; j++) {
				mess.mav1.crc[i] = j;
				if (j != old) {
					sprintf(fname, "pass.%d", passSeed++);
					
					if ((fp = fopen(fname, "wb")) == NULL) {
						fprintf(stderr, "Failed to open file.\n");
						return;
					}
					
					writeMessageToFileMav(&mess, fp);
					
					fclose(fp);
					
				}
			}
			mess.mav1.crc[i] = old;
		}
		
		// Generate a failing test case by writing one too many bytes to a file.
		sprintf(fname, "fail.%d", failSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeAltFormatMessageToFileMav(&mess, fp, 1, mess.mav1.payloadLen, mess.mav1.payloadLen + 9);
		
		fclose(fp);

		// Generate a failing test case by writing one too few bytes to a file.
		sprintf(fname, "fail.%d", failSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeAltFormatMessageToFileMav(&mess, fp, 1, mess.mav1.payloadLen, mess.mav1.payloadLen + 7);
		
		fclose(fp);
		
		free(mess.mav1.payload);
	}
	
	// Create a MAVLINK 2 message with random byte values in wildcard fields and all other fields fixed.
	mess.mav2.mavCode = 0xfd;
	mess.mav2.payloadLen = (uint8_t)maxLenM2;
	mess.mav2.incompFlag = 0;
	mess.mav2.compFlag = 0;
	mess.mav2.packetSeq = (uint8_t)(rand() % 256);
	mess.mav2.systemID = (uint8_t)(rand() % 256);
	mess.mav2.compID = (uint8_t)(rand() % 256);
	fromInt24le(msgID, mess.mav2.messageID);
	
	// Allocate memory for the payload.
	if ((mess.mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*maxLenM2)) == NULL)
		fprintf(stderr, "Memory allocation failed.\n");
	
	// Fill payload bytes.
	for (i = 0; i < maxLenM2; i++) {
		// Generate a random byte for wildcard fields.
		randomByte = (uint8_t)(rand() % 256);

		// Differentiate between ennumerated and non-ennumerated fields.
		if (!(vectorContains(enFields, compareInt, (void *)&i))) mess.mav2.payload[i] = randomByte;
		else mess.mav2.payload[i] = fillEnumInRange(msgID, i);
	}
		
	// Fill in the crc with random bytes.
	mess.mav2.crc[0] = (uint8_t)(rand() % 256);
	mess.mav2.crc[1] = (uint8_t)(rand() % 256);
	mess.mav2.signedMess = false;
	
	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	// Generate failing tests for MAVLink code.
	old = mess.mav2.mavCode;
	for (i = 0; i <= 255; i++) {
		mess.mav2.mavCode = (uint8_t)i;
		if (i != old) {
			// Generate a single passing test.
			sprintf(fname, "fail.%d", failSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 12);
			
			fclose(fp);
			
		}
	}
	mess.mav2.mavCode = old;

	// Generate failing tests for each payload length.
	old = mess.mav2.payloadLen;
	for (i = 1; i <= maxLenM2; i++) {
		mess.mav2.payloadLen = (uint8_t)i;

		// Write a passing test for the payload length
		sprintf(fname, "pass.%d", passSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeMessageToFileMav(&mess, fp);
			
		fclose(fp);

		// Generate failing tests for the rest of the payload lengths.
		for (j = 1; j <= maxLenM2; j++) {
			if (j != i) {
				// Generate a single failing test.
				sprintf(fname, "fail.%d", failSeed++);
				
				if ((fp = fopen(fname, "wb")) == NULL) {
					fprintf(stderr, "Failed to open file.\n");
					return;
				}
				
				writeAltFormatMessageToFileMav(&mess, fp, 2, j, j + 12);
				
				fclose(fp);
			}
		}
	}
	mess.mav2.payloadLen = old;
		
	// Generate failing tests for incompatibility flag.
	old = mess.mav2.incompFlag;
	for (i = 0; i <= 255; i++) {
		mess.mav2.incompFlag = (uint8_t)i;
		if (i != old) {
			// Generate a single passing test.
			sprintf(fname, "fail.%d", failSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
			
		}
	}
	mess.mav2.incompFlag = old;

	// Generate failing tests for compatibility flag.
	old = mess.mav2.compFlag;
	for (i = 0; i <= 255; i++) {
		mess.mav2.compFlag = (uint8_t)i;
		if (i != old) {
			// Generate a single passing test.
			sprintf(fname, "fail.%d", failSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
			
		}
	}
	mess.mav2.compFlag = old;

	// Generate passing tests for packet sequence.
	old = mess.mav2.packetSeq;
	for (i = 0; i <= 255; i++) {
		mess.mav2.packetSeq = (uint8_t)i;
		if (i != old) {
			// Generate a single passing test.
			sprintf(fname, "pass.%d", passSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
			
		}
	}
	mess.mav2.packetSeq = old;
	
	// Generate passing tests for system ID.
	old = mess.mav2.systemID;
	for (i = 0; i <= 255; i++) {
		mess.mav2.systemID = (uint8_t)i;
		if (i != old) {
			// Generate a single passing test.
			sprintf(fname, "pass.%d", passSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
			
		}
	}
	mess.mav2.systemID = old;
	
	// Generate passing tests for component ID.
	old = mess.mav2.compID;
	for (i = 0; i <= 255; i++) {
		mess.mav2.compID = (uint8_t)i;
		if (i != old) {
			// Generate a single passing test.
			sprintf(fname, "pass.%d", passSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
			
		}
	}
	mess.mav2.compID = old;
	
	// Generate passing and failing tests of all payload fields.
	for (i = 0; i < maxLenM2; i++) {
		old = mess.mav2.payload[i];
		// First deal with non-enumerated fields.
		if (!(vectorContains(enFields, compareInt, (void *)&i))) {
			for (j = 0; j < 255; j++) {
				mess.mav2.payload[i] = j;
				if (j != old) {
					// Generate a single passing test.
					sprintf(fname, "pass.%d", passSeed++);
					
					if ((fp = fopen(fname, "wb")) == NULL) {
						fprintf(stderr, "Failed to open file.\n");
						return;
					}
					
					writeMessageToFileMav(&mess, fp);
					
					fclose(fp);
					
				}
			}
		}
		else {
			// Generate tests for enumerated fields.
			generateEnumTests(msgID, i, mess, maxLenM2, &passSeed, &failSeed);
		}	
		mess.mav2.payload[i] = old;
	}
	
	// Generate passing tests for all fields of CRC.
	for (i = 0; i < 2; i++) {
		old = mess.mav2.crc[i];
		for (j = 0; j < 255; j++) {
			mess.mav2.crc[i] = j;
			if (j != old) {
				// Generate a single passing test.
				sprintf(fname, "pass.%d", passSeed++);
				
				if ((fp = fopen(fname, "wb")) == NULL) {
					fprintf(stderr, "Failed to open file.\n");
					return;
				}
				
				writeMessageToFileMav(&mess, fp);
				
				fclose(fp);
				
			}
		}
		mess.mav2.crc[i] = old;
	}
	
	// Generate a failing test case by writing one too many bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 13);
		
	fclose(fp);
	
	// Generate a failing test case by writing one too few bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 11);
	
	fclose(fp);

	// Test signed message.
	mess.mav2.signedMess = true;
	mess.mav2.incompFlag = 0x01;

	// Fill in signature field.
	for (i = 0; i < 13; i++)
		mess.mav2.signature[i] = (uint8_t)(rand() % 256);

	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	// Generate failing tests for incompatibility flag.
	old = mess.mav2.incompFlag;
	for (i = 0; i <= 255; i++) {
		mess.mav2.incompFlag = (uint8_t)i;
		if (i != old) {
			// Generate a single passing test.
			sprintf(fname, "fail.%d", failSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);
			
		}
	}
	mess.mav2.incompFlag = old;

	// Generate a failing test case by writing one too many bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 26);
	
	fclose(fp);
	
	// Generate a failing test case by writing one too few bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 24);
	
	fclose(fp);
	
	free(mess.mav2.payload);
	
	// Clean up memory.
	vectorApply(enFields, free);
	vectorFree(enFields);
	
}

/*
 * Gwenerate a smaller suite of tests for a given message id.
 * Inputs: message ID, initial pass test number, initial fail test number
 * Outputs: none
 */
void generateTestsShort(int msgID, int passSeed, int failSeed) {
	// Variable declarations.
	int maxLenM1, maxLenM2, i;
	FILE *fp;
	char fname[50];
	vector_t *enFields;
	mavMessage_t mess;
	uint8_t randomByte, old;
	
	// Seed a random number generator.
	srand(time(NULL));

	// Initialise the given message type.
	if ((enFields = initialiseMessageType(msgID, &maxLenM1, &maxLenM2)) == NULL) {
		fprintf(stderr, "Error initialising message type.\n");
		return;
	}
	
	// Only test MAVLink 1 messages for message IDs under 255.
	if (msgID <= 255) {
		// Create a message with random byte values in wildcard fields and all other fields fixed.
		mess.mav1.mavCode = 0xfe;
		mess.mav1.payloadLen = (uint8_t)maxLenM1;
		mess.mav1.packetSeq = (uint8_t)(rand() % 256);
		mess.mav1.systemID = (uint8_t)(rand() % 256);
		mess.mav1.compID = (uint8_t)(rand() % 256);
		mess.mav1.messageID = (uint8_t)msgID;
		
		// Allocate memory for the payload.
		if ((mess.mav1.payload = (uint8_t *)malloc(sizeof(uint8_t)*maxLenM1)) == NULL)
			fprintf(stderr, "Memory allocation failed.\n");

		// Fill payload bytes.
		for (i = 0; i < maxLenM1; i++) {
			// Generate a random byte for wildcard fields.
			randomByte = (uint8_t)(rand() % 256);

			// Differentiate between ennumerated and non-ennumerated fields.
			if (!(vectorContains(enFields, compareInt, (void *)&i))) mess.mav1.payload[i] = randomByte;
			else mess.mav1.payload[i] = fillEnumInRange(msgID, i);
		}
		
		// Fill in the crc with random bytes.
		mess.mav1.crc[0] = (uint8_t)(rand() % 256);
		mess.mav1.crc[1] = (uint8_t)(rand() % 256);

		// Generate a single passing test.
		sprintf(fname, "pass.%d", passSeed++);
				
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}

		writeMessageToFileMav(&mess, fp);

		fclose(fp);
		
		// Generate a failing test for MAVLink code.
		old = mess.mav1.mavCode;
		mess.mav1.mavCode = 0xfd;

		// Generate a single failing test.
		sprintf(fname, "fail.%d", failSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeAltFormatMessageToFileMav(&mess, fp, 1, mess.mav1.payloadLen, mess.mav1.payloadLen + 8);
		
		fclose(fp);
		
		mess.mav1.mavCode = old;

		// Generate failing test for payload length.
		old = mess.mav1.payloadLen;
		mess.mav1.payloadLen = old + 0x01;

		// Generate a single failing test.
		sprintf(fname, "fail.%d", failSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeAltFormatMessageToFileMav(&mess, fp, 1, old, old + 8);
		
		fclose(fp);
		
		mess.mav1.payloadLen = old;

		// Generate passing test for packet sequence.
		old = mess.mav1.packetSeq;
		mess.mav1.packetSeq = old + 0x01;

		// Generate a single passing test.
		sprintf(fname, "pass.%d", passSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeMessageToFileMav(&mess, fp);
		
		fclose(fp);
		
		mess.mav1.packetSeq = old;

		// Generate passing test for system ID.
		old = mess.mav1.systemID;
		mess.mav1.systemID = (uint8_t)old + 0x01;

		// Generate a single passing test.
		sprintf(fname, "pass.%d", passSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
				
		writeMessageToFileMav(&mess, fp);
		
		fclose(fp);
		
		mess.mav1.systemID = old;
		
		// Generate passing test for component ID.
		old = mess.mav1.compID;
		mess.mav1.compID = (uint8_t)i;

		// Generate a single passing test.
		sprintf(fname, "pass.%d", passSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeMessageToFileMav(&mess, fp);
		
		fclose(fp);
		
		mess.mav1.compID = old;
		
		// Generate passing tests for all fields of CRC.
		for (i = 0; i < 2; i++) {
			old = mess.mav1.crc[i];
			mess.mav1.crc[i] = old + 0x01;

			sprintf(fname, "pass.%d", passSeed++);
			
			if ((fp = fopen(fname, "wb")) == NULL) {
				fprintf(stderr, "Failed to open file.\n");
				return;
			}
			
			writeMessageToFileMav(&mess, fp);
			
			fclose(fp);

			mess.mav1.crc[i] = old;
		}
		
		// Generate a failing test case by writing one too many bytes to a file.
		sprintf(fname, "fail.%d", failSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeAltFormatMessageToFileMav(&mess, fp, 1, mess.mav1.payloadLen, mess.mav1.payloadLen + 9);
		
		fclose(fp);

		// Generate a failing test case by writing one too few bytes to a file.
		sprintf(fname, "fail.%d", failSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeAltFormatMessageToFileMav(&mess, fp, 1, mess.mav1.payloadLen, mess.mav1.payloadLen + 7);
		
		fclose(fp);
		
		free(mess.mav1.payload);
	}
	
	// Create a MAVLINK 2 message with random byte values in wildcard fields and all other fields fixed.
	mess.mav2.mavCode = 0xfd;
	mess.mav2.payloadLen = (uint8_t)maxLenM2;
	mess.mav2.incompFlag = 0;
	mess.mav2.compFlag = 0;
	mess.mav2.packetSeq = (uint8_t)(rand() % 256);
	mess.mav2.systemID = (uint8_t)(rand() % 256);
	mess.mav2.compID = (uint8_t)(rand() % 256);
	fromInt24le(msgID, mess.mav2.messageID);
	
	// Allocate memory for the payload.
	if ((mess.mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*maxLenM2)) == NULL)
		fprintf(stderr, "Memory allocation failed.\n");
	
	// Fill payload bytes.
	for (i = 0; i < maxLenM2; i++) {
		// Generate a random byte for wildcard fields.
		randomByte = (uint8_t)(rand() % 256);

		// Differentiate between ennumerated and non-ennumerated fields.
		if (!(vectorContains(enFields, compareInt, (void *)&i))) mess.mav2.payload[i] = randomByte;
		else mess.mav2.payload[i] = fillEnumInRange(msgID, i);
	}
		
	// Fill in the crc with random bytes.
	mess.mav2.crc[0] = (uint8_t)(rand() % 256);
	mess.mav2.crc[1] = (uint8_t)(rand() % 256);
	mess.mav2.signedMess = false;
	
	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	// Generate failing test for MAVLink code.
	old = mess.mav2.mavCode;
	mess.mav2.mavCode = 0xfe;

	// Generate a single failing test.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 12);
	
	fclose(fp);
	
	mess.mav2.mavCode = old;
	
	// Generate failing tests for each payload length.
	old = mess.mav2.payloadLen;
	for (i = 1; i <= maxLenM2; i++) {
		mess.mav2.payloadLen = (uint8_t)i;

		// Write a passing test for the payload length
		sprintf(fname, "pass.%d", passSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeMessageToFileMav(&mess, fp);
			
		fclose(fp);
		
		// Generate a failing test for the given of the payload lengths.
		sprintf(fname, "fail.%d", failSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen + 1, mess.mav2.payloadLen + 13);
		
		fclose(fp);
	}
	mess.mav2.payloadLen = old;
		
	// Generate failing tests for incompatibility flag.
	old = mess.mav2.incompFlag;
	mess.mav2.incompFlag = 0x01;

	// Generate a single failing test.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);

	mess.mav2.incompFlag = old;
	
	// Generate failing test for compatibility flag.
	old = mess.mav2.compFlag;
	mess.mav2.compFlag = old + 0x01;
	
	// Generate a single failing test.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
			
	fclose(fp);
	
	mess.mav2.compFlag = old;
	
	// Generate passing tests for packet sequence.
	old = mess.mav2.packetSeq;
	mess.mav2.packetSeq = old + 0x01;
	
	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	mess.mav2.packetSeq = old;
	
	// Generate passing tests for system ID.
	old = mess.mav2.systemID;
	mess.mav2.systemID = old + 0x01;

	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	mess.mav2.systemID = old;
	
	// Generate passing tests for component ID.
	old = mess.mav2.compID;
	mess.mav2.compID = old + 0x01;
	
	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	mess.mav2.compID = old;
	
	// Generate passing tests for all fields of CRC.
	for (i = 0; i < 2; i++) {
		old = mess.mav2.crc[i];
		mess.mav2.crc[i] = old + 0x01;

		// Generate a single passing test.
		sprintf(fname, "pass.%d", passSeed++);
		
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			return;
		}
		
		writeMessageToFileMav(&mess, fp);
		
		fclose(fp);
		
		mess.mav2.crc[i] = old;
	}
	
	// Generate a failing test case by writing one too many bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 13);
	
	fclose(fp);
	
	// Generate a failing test case by writing one too few bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 11);
	
	fclose(fp);

	// Test signed message.
	mess.mav2.signedMess = true;
	mess.mav2.incompFlag = 0x01;

	// Fill in signature field.
	for (i = 0; i < 13; i++)
		mess.mav2.signature[i] = (uint8_t)(rand() % 256);

	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	// Generate failing tests for incompatibility flag.
	old = mess.mav2.incompFlag;
	mess.mav2.incompFlag = 0x00;

	// Generate a single passing test.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	mess.mav2.incompFlag = old;

	// Generate a failing test case by writing one too many bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 26);
	
	fclose(fp);
	
	// Generate a failing test case by writing one too few bytes to a file.
	sprintf(fname, "fail.%d", failSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeAltFormatMessageToFileMav(&mess, fp, 2, mess.mav2.payloadLen, mess.mav2.payloadLen + 24);
	
	fclose(fp);
	
	free(mess.mav2.payload);
	
	// Clean up memory.
	vectorApply(enFields, free);
	vectorFree(enFields);
	
}

/*
 * Generate a single passing test for a given message type.
 * Inputs: message ID, pass test number
 * Outputs: none
 */
void generateSingleTest(int msgID, int passSeed) {
	// Variable declarations.
	int maxLenM1, maxLenM2, i;
	FILE *fp;
	char fname[50];
	vector_t *enFields;
	mavMessage_t mess;
	uint8_t randomByte;
	
	// Seed a random number generator.
	srand(time(NULL));

	// Initialise the given message type.
	if ((enFields = initialiseMessageType(msgID, &maxLenM1, &maxLenM2)) == NULL) {
		fprintf(stderr, "Error initialising message type.\n");
		return;
	}
		
	// Create a MAVLINK 2 message with random byte values in wildcard fields and all other fields fixed.
	mess.mav2.mavCode = 0xfd;
	mess.mav2.payloadLen = (uint8_t)maxLenM2;
	mess.mav2.incompFlag = 0;
	mess.mav2.compFlag = 0;
	mess.mav2.packetSeq = (uint8_t)(rand() % 256);
	mess.mav2.systemID = (uint8_t)(rand() % 256);
	mess.mav2.compID = (uint8_t)(rand() % 256);
	fromInt24le(msgID, mess.mav2.messageID);
	
	// Allocate memory for the payload.
	if ((mess.mav2.payload = (uint8_t *)malloc(sizeof(uint8_t)*maxLenM2)) == NULL)
		fprintf(stderr, "Memory allocation failed.\n");
	
	// Fill payload bytes.
	for (i = 0; i < maxLenM2; i++) {
		// Generate a random byte for wildcard fields.
		randomByte = (uint8_t)(rand() % 256);

		// Differentiate between ennumerated and non-ennumerated fields.
		if (!(vectorContains(enFields, compareInt, (void *)&i))) mess.mav2.payload[i] = randomByte;
		else mess.mav2.payload[i] = fillEnumInRange(msgID, i);
	}
		
	// Fill in the crc with random bytes.
	mess.mav2.crc[0] = (uint8_t)(rand() % 256);
	mess.mav2.crc[1] = (uint8_t)(rand() % 256);
	mess.mav2.signedMess = false;
	
	// Generate a single passing test.
	sprintf(fname, "pass.%d", passSeed++);
	
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return;
	}
	
	writeMessageToFileMav(&mess, fp);
	
	fclose(fp);
	
	free(mess.mav2.payload);
	
	// Clean up memory.
	vectorApply(enFields, free);
	vectorFree(enFields);
	
}
