/* 
 * analyze.c -- runs analysis on the data captured in a .cap file
 * 
 * Author: Joshua M. Meise
 * Created: 06-26-2024
 * Version: 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define MAVLEN 48
#define HEARTBEAT 0
#define SYSTEM_STATUS 1
#define PING 4
#define LINK_NODE_STATUS 8
#define PARAM_VALUE 22
#define GPS_RAW_INT 24
#define SCALED_PRESSURE 29
#define ATTITUDE 30
#define ATTITUDE_QUATERNATION 31
#define LOCAL_POSITION_NED 32
#define GLOBAL_POSITION_INT 33
#define RC_CHANNELS 65
#define VFR_HUD 74
#define ATTITUDE_TARGET 83
#define POSITION_TARGET_LOCAL_NED 85
#define RADIO_STATUS 109
#define ALTITUDE 141
#define BATTERY_STATUS 147
#define ESTIMATOR_STATUS 230
#define VIBRATION 241
#define HOME_POSITION 242
#define EXTENDED_SYSSTATE 245
#define STATUSTEXT 253
#define UTM_GLOBAL_POSITION 340
#define TIME_ESTIMATE_TO_TARGET 380
#define EVENT 410
#define CURRENT_EVENT_SEQUENCE 411
#define OPEN_DRONE_ID_LOCATION 12901
#define OPEN_DRONE_ID_SYSTEM 12904

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
} messageBody_t;

// Each message contains a header and a body.
typedef struct message {
	messageHeader_t header;
	messageBody_t body;
} message_t;

/* 
 * Converts a 16 bit hexadecimal number (stored in an array to an integer.
 * Inputs: the array
 * Outputs: the integer value of the length of the message
 */
static int toInt16(uint8_t *arr) {
	return (int)arr[1] + (int)arr[0]*256;
}

/* 
 * Converts a 32 bit hexadecimal number (stored in an array to an integer.
 * Inputs: the start index of the array, how many bytes are in the hex representation of the number
 * Outputs: the integer value of the length of the message
 */
static int toInt32(uint8_t *arr) {
	return (int)arr[0] + (int)arr[1]*256 + (int)arr[2]*4096 + (int)arr[3]*65536;
}


/*
 * Prints out all of the unique codes (hexadecimal values and decimal values).
 * Inputs: message, length of message, length of payload.
 * Outputs: none.
 */
static void printMessage(message_t mess, int len, int payload) {
	// Variable declarations.
	int i;
	uint8_t buf;

	printf("Message\n" );
	
	for (i = 0; i < len; i++) {
		if (i < 4) buf = mess.header.timeS[i];
		else if (i < 8) buf = mess.header.timeUNS[i - 4];
		else if (i < 12) buf = mess.header.capturedPacketLength[i - 8];
		else if (i < 16) buf = mess.header.originalPacketLength[i - 12];
		else if (i < 20) buf = mess.header.messageFam[i - 16];
		else if (i == 20) buf = mess.header.iphl;
		else if (i == 21) buf = mess.header.dsf;
		else if (i < 24) buf = mess.header.totalLength[i - 22];
		else if (i < 26) buf = mess.header.id[i - 24];
		else if (i < 28) buf = mess.header.ffOffset[i - 26];
		else if (i == 28) buf = mess.header.ttl;
		else if (i == 29) buf = mess.header.protocol;
		else if (i < 32) buf = mess.header.headerChecksum[i - 30];
		else if (i < 36) buf = mess.header.sourceAddr[i - 32];
		else if (i < 40) buf = mess.header.destAddr[i - 36];
		else if (i < 42) buf = mess.header.sourcePort[i - 40];
		else if (i < 44) buf = mess.header.destPort[i - 42];
		else if (i < 46) buf = mess.header.length[i - 44];
		else if (i < 48) buf = mess.header.checkSum[i - 46];
		else if (i == MAVLEN) buf = mess.body.mavCode;
		else if (i == MAVLEN + 1) buf =	mess.body.payloadLen;
		else if (i == MAVLEN + 2) buf = mess.body.incompFlag;
		else if (i == MAVLEN + 3) buf = mess.body.compFlag;
		else if (i == MAVLEN + 4) buf = mess.body.packetSeq;
		else if (i == MAVLEN + 5) buf = mess.body.systemID;
		else if (i == MAVLEN + 6) buf = mess.body.compID;
		else if (i < MAVLEN + 10) buf = mess.body.messageID[i - MAVLEN - 7];		
		else if (i < MAVLEN + payload + 10) buf = mess.body.payload[i - MAVLEN - 10];
		else buf = mess.body.crc[i - MAVLEN - payload - 10];

		printf("%02x ", buf);
	}

	printf("\n");

}

int main(int argc, char **argv) {
	// Variable declarations
	FILE *ifile;
	uint8_t buf;
	int bytesRead, i, payload, numMess;
	message_t *messArr, mess;

	// Check arguments
	if (argc != 2) {
    printf("usage: countcodes inputFile\n");
    exit(EXIT_FAILURE);
	}

	// Check that provided file exists by opening for binary reading
	if ((ifile = fopen(argv[1], "rb")) == NULL) {
		printf("usage: countcodes inputFile\n");
		exit(EXIT_FAILURE);
	}
	
	i = 0;

	// Read the first 24 bytes of the file - file header.
	while(i < 24) {
		bytesRead = fread(&buf, sizeof(buf), 1, ifile);
		i++;
	}

	i = 0;
	payload = 0;
	numMess = 0;

	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		if (i < 4) mess.header.timeS[i] = buf;
		else if (i < 8) mess.header.timeUNS[i - 4] = buf;
		else if (i < 12) mess.header.capturedPacketLength[i - 8] = buf;
		else if (i < 16) mess.header.originalPacketLength[i - 12] = buf;
		else if (i < 20) mess.header.messageFam[i - 16] = buf;
		else if (i == 20) mess.header.iphl = buf;
		else if (i == 21) mess.header.dsf = buf;
		else if (i < 24) mess.header.totalLength[i - 22] = buf;
		else if (i < 26) mess.header.id[i - 24] = buf;
		else if (i < 28) mess.header.ffOffset[i - 26] = buf;
		else if (i == 28) mess.header.ttl = buf;
		else if (i == 29) mess.header.protocol = buf;
		else if (i < 32) mess.header.headerChecksum[i - 30] = buf;
		else if (i < 36) mess.header.sourceAddr[i - 32] = buf;
		else if (i < 40) mess.header.destAddr[i - 36] = buf;
		else if (i < 42) mess.header.sourcePort[i - 40] = buf;
		else if (i < 44) mess.header.destPort[i - 42] = buf;
		else if (i < 46) mess.header.length[i - 44] = buf;
		else if (i < 48) mess.header.checkSum[i - 46] = buf;
		else if (i == MAVLEN) mess.body.mavCode = buf;
		else if (i == MAVLEN + 1) {
			mess.body.payloadLen = buf;

			payload = (int)buf;
			
			if ((mess.body.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL)
				printf("Memory allocation failed.\n");
		}
		else if (i == MAVLEN + 2) mess.body.incompFlag = buf;
		else if (i == MAVLEN + 3) mess.body.compFlag = buf;
		else if (i == MAVLEN + 4) mess.body.packetSeq = buf;
		else if (i == MAVLEN + 5) mess.body.systemID = buf;
		else if (i == MAVLEN + 6) mess.body.compID = buf;
		else if (i < MAVLEN + 10) mess.body.messageID[i - MAVLEN - 7] = buf;		
		else if (i < MAVLEN + payload + 10) mess.body.payload[i - MAVLEN - 10] = buf;
		else mess.body.crc[i - MAVLEN - payload - 10] = buf;		
		
		i++;

		// If the end of a message has been reached.
		if (i == (MAVLEN + payload + 12) && payload != 0) {
			if (numMess == 0) 
				messArr = (message_t *)malloc(sizeof(message_t));
			else 
				messArr = (message_t *)realloc(messArr, sizeof(message_t)*(numMess + 1));

			messArr[numMess] = mess;
					
			// Flag messages for mismatching lengths.
			if (toInt32(mess.header.capturedPacketLength) != toInt32(mess.header.originalPacketLength)) printf("flag1\n");
			else if (payload != toInt32(mess.header.capturedPacketLength) - 44) printf("flag2\n");
			else if (toInt16(mess.header.totalLength) != toInt32(mess.header.capturedPacketLength) - 4) printf("flag3\n");
			
			// Reset all variables
			i = 0;
			payload = 0;
			numMess++;
		}
		
	}

	for (i = 0; i < numMess; i++)
		free(messArr[i].body.payload);

	free(messArr);
	
	fclose(ifile);
	exit(EXIT_SUCCESS);
}
