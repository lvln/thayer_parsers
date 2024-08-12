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

// Predefined constants.
#define HEARTBEAT 0
#define SYS_STATUS 1
#define PING 4
#define LINK_NODE_STATUS 8
#define PARAM_VALUE 22
#define GPS_RAW_INT 24
#define SCALED_PRESSURE 29
#define ATTITUDE 30
#define ATTITUDE_QUATERNION 31
#define LOCAL_POSITION_NED 32
#define GLOBAL_POSITION_INT 33
#define MISSION_CURRENT 42
#define MISSION_ITEM_REACHED 46
#define RC_CHANNELS 65
#define VFR_HUD 74
#define COMMAND_ACK 77
#define ATTITUDE_TARGET 83
#define POSITION_TARGET_LOCAL_NED 85
#define POSITION_TARGET_GLOBAL_INT 87
#define RADIO_STATUS 109
#define ALTITUDE 141
#define BATTERY_STATUS 147
#define ESTIMATOR_STATUS 230
#define VIBRATION 241
#define HOME_POSITION 242
#define EXTENDED_SYS_STATE 245
#define STATUSTEXT 253
#define UTM_GLOBAL_POSITION 340
#define TIME_ESTIMATE_TO_TARGET 380
#define EVENT 410
#define CURRENT_EVENT_SEQUENCE 411
#define OPEN_DRONE_ID_LOCATION 12901
#define OPEN_DRONE_ID_SYSTEM 12904

#define HEARTBEAT_LEN 9
#define SYS_STATUS_LEN 43
#define PING_LEN 14
#define LINK_NODE_STATUS_LEN 36
#define PARAM_VALUE_LEN 25
#define GPS_RAW_INT_LEN 52
#define SCALED_PRESSURE_LEN 16
#define ATTITUDE_LEN 28
#define ATTITUDE_QUATERNION_LEN 48
#define LOCAL_POSITION_NED_LEN 28
#define GLOBAL_POSITION_INT_LEN 28
#define MISSION_CURRENT_LEN 18
#define MISSION_ITEM_REACHED_LEN 2
#define RC_CHANNELS_LEN 42
#define VFR_HUD_LEN 20
#define COMMAND_ACK_LEN 10
#define ATTITUDE_TARGET_LEN 37
#define POSITION_TARGET_LOCAL_NED_LEN 51
#define POSITION_TARGET_GLOBAL_INT_LEN 51
#define RADIO_STATUS_LEN 9
#define ALTITUDE_LEN 32
#define BATTERY_STATUS_LEN 54
#define ESTIMATOR_STATUS_LEN 42
#define VIBRATION_LEN 32
#define HOME_POSITION_LEN 60
#define EXTENDED_SYS_STATE_LEN 2
#define STATUSTEXT_LEN 54
#define UTM_GLOBAL_POSITION_LEN 70
#define TIME_ESTIMATE_TO_TARGET_LEN 20
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
