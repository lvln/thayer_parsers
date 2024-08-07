/* 
 * mavlink.h --- header file for fuunctions to operate on mavlink data
 * 
 * Author: Joshua M. Meise
 * Created: 06-27-2024
 * Version: 1.0
 * 
 * Description: 
 * 
 */

// Library inclusions.
#include <stdint.h>
#include <stdio.h>

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

// Define the message header with the various fields.
typedef struct messageHeader {
	uint8_t timeS[4];
	uint8_t timeUNS[4];
	uint8_t capturedPacketLength[4];
	uint8_t originalPacketLength[4];
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
} messageHeader_t;

// Define the message body with the various fields.
typedef struct messageBody {
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
	uint8_t signature[13];
} messageBody_t;

// Each message contains a header and a body.
typedef struct message {
	messageHeader_t header;
	messageBody_t body;
} message_t;

// Data structure to hold array of MAVLink messages and length.
typedef struct mavMessArr {
	messageBody_t *messages;
	int n;
} mavMessArr_t;

// Data structure to hold array of pcap messages and length.
typedef struct pcapMessArr {
	message_t *pcap;
	int n;
} messArr_t;

// Data structure to hold a pcap file.
typedef struct pcap {
	uint8_t header[24];
	message_t *messages;
	int n;
} pcap_t;

/*
 * Convert from 8 bit hexadecimal to integer.
 * Inputs: hex number.
 * Outputs: integer enquvalent.
 */
int toInt8(uint8_t *arr);

/*
 * Convert from 16 bit hexadecimal (little endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt16le(uint8_t *arr);

/*
 * Convert from 16 bit hexadecimal (big endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt16be(uint8_t *arr);

/*
 * Convert from 24 bit hexadecimal (little endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt24le(uint8_t *arr);

/*
 * Convert from 24 bit hexadecimal (big endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt24be(uint8_t *arr);

/*
 * Convert from integer to 24 bit hexadecimal (little endian).
 * Inputs: integer value, array into which to write hex value
 * Outputs: none
 */
void fromInt24le(int num, uint8_t arr[]);

/*
 * Writes a pcap header to a file.
 * Inputs: pcap file data structure, file to which to write.
 * Outputs: none.
 */	
void writeHeader(pcap_t *pcapFile, FILE *ofile);

/*
 * Write raw MAVLink message to a file - just the header and payload.
 * Inputs: message to write, file pointer.
 * Outputs: none.
 */
void writeMessageToFileMav(messageBody_t mess, FILE *ofile);

/*
 * Write a MAVLink message to a file with PCAP wrappers.
 * Inputs: message to write, file pointer.
 * Outputs: none.
 */
void writeMessageToFilePcap(message_t mess, FILE *ofile);

/*
 * Writes a specified number of bytes of a MAVLink message to a file.
 * Inputs: message to write, pointer to file to write, number of bytes to write, payload length of the message
 * Outputs: none
 */
void writeMavDiffLenToFile(messageBody_t mess, FILE *fp, int len, int payload);

/*
 * Print out a message without it's PCAP wrappers.
 * Inputs: MAVLink message to print
 * Outputs: none.
 */
void printMessageMav(messageBody_t mess);

/*
 * Print out a message incuding its PCAP wrappers.
 * Inputs: message to print
 * Outputs: none.
 */
void printMessagePcap(message_t mess);

/*
 * Read a file that contains just MAVLink messagess - no pcap header or pcap wrappers.
 * Inputs: input file
 * Outputs: array of messages; NULL if unsuccessful
 */
mavMessArr_t *readFileMav(FILE *ifile);

/*
 * Read a pcap file (header and all messages.
 * Inputs: input file
 * Outputs: pcap file data structure; NULL if unsuccessful
 */
pcap_t *readFilePcap(FILE *ifile);

/*
 * Re-inserts the trailing zeros in a MAVLink message to standardise the format for parsing.
 * Inputs: message
 * Outputs: updated message
 */
void untruncate(message_t *mess);

/*
 * Free memory from payload, messages array and the structure itself for mavlink messages.
 * Inputs: MAVLink message array structure.
 * Outputs: none.
 */
void freeMemMav(mavMessArr_t *messArr);

/*
 * Free memory from payload, messages array and the structure itself for a pcap file structure.
 * Inputs: pcap file structure.
 * Outputs: none.
 */
void freeMemPcap(pcap_t *pcapFile);

/*
 * Generats test cases for a given message id.
 * Inputs: message id
 * Outputs: none
 */
void generateTests(int msgID);
