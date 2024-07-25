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
#include <time.h>
#include <string.h>

/*
 * Checks if a given value is found within an array.
 * Inputs: array of values, value to find
 * Outputs: true if found, false otherwise
 */
static bool arrContains(arrSize_t arr, int val) {
	// Variable declarations.
	int i;

	for (i = 0; i < arr.n; i++) {
		if (arr.arr[i] == val)
			return true;
	}

	return false;
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
	if (msgID == POSITION_TARGET_GLOBAL_INT && ind == 50)
		val = 0x00;
	else
		val = 0x01;

	return val;
}

/*
 * Writes the actual tests to files based on valid ennumerated values.
 * Inputs: message to write to file, field index, valid enumerated values, maximum length of message, message ID
 * Outputs: none
 */
static void gt(messageBody_t mess, int ind, arrSize_t arr, int maxLen, int msgID) {
	// Variable declarations.
	int i;
	char fname[50];
	FILE *fp;
	
	for (i = 0; i <= 255; i++) {
		mess.payload[ind] = (uint8_t)i;
		
		// If a valid enumrated value, make it a pass test.
		if (arrContains(arr, i)) {
			// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
			sprintf(fname, "./pass.%d.%d.%d.%d", msgID, maxLen, ind, i);
			
			// Open file for writing.
			if ((fp = fopen(fname, "wb")) == NULL) {
				printf("Error opening file %s for writing.\n", fname);
				exit(EXIT_FAILURE);
			}
			
			// Write message to file.
			writeMavMessageToFile(mess, fp);
			
			fclose(fp);
		}
		// Else make it a failure test.
		else {
			// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
			sprintf(fname, "./fail.%d.%d.%d.%d", msgID, maxLen, ind, i);
			
			// Open file for writing.
			if ((fp = fopen(fname, "wb")) == NULL) {
				printf("Error opening file %s for writing.\n", fname);
				exit(EXIT_FAILURE);
			}
			
			// Write message to file.
			writeMavMessageToFile(mess, fp);
			
			fclose(fp);
		}
	}
}

/*
 * Generates passing and failing tests for enumerated fields
 * Inputs: message ID, field index, message
 * Outputs: none
 */
static void generateEnumTests(int msgID, int ind, messageBody_t mess) {
  // Variable declarations.
	arrSize_t arr;
	int i;

	// Fill in the valid enumerated values for each message type and field.
	switch(msgID) {
	case HEARTBEAT:
		if (ind == 4) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*45)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 45;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, HEARTBEAT_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 5) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*21)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 21;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, HEARTBEAT_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 7) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*9)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 9;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, HEARTBEAT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case PARAM_VALUE:
		if (ind == 24) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*10)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 10;

			// Fill array.
			for (i = 1; i <= arr.n; i++)
				arr.arr[i - 1] = i;

			// Generate the test files.
			gt(mess, ind, arr, PARAM_VALUE_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case GPS_RAW_INT:
		if (ind == 28) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*9)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 9;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, GPS_RAW_INT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case MISSION_CURRENT:
		if (ind == 4) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*6)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 6;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, MISSION_CURRENT_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 5) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 3;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, MISSION_CURRENT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case COMMAND_ACK:
		if (ind == 2) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*10)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 10;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, COMMAND_ACK_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case POSITION_TARGET_LOCAL_NED:
		if (ind == 50) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*4)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 4;

			// Fill array.
			arr.arr[0] = 0x01;
			arr.arr[1] = 0x07;
			arr.arr[2] = 0x08;
			arr.arr[3] = 0x09;

			// Generate the test files.
			gt(mess, ind, arr, POSITION_TARGET_LOCAL_NED_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case POSITION_TARGET_GLOBAL_INT:
		if (ind == 50) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*6)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 6;

			// Fill array.
			arr.arr[0] = 0x01;
			arr.arr[1] = 0x03;
			arr.arr[2] = 0x0A;
			arr.arr[3] = 0x05;
			arr.arr[4] = 0x06;
			arr.arr[5] = 0x0B;

			// Generate the test files.
			gt(mess, ind, arr, POSITION_TARGET_GLOBAL_INT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case BATTERY_STATUS:
		if (ind == 33) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 34) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 40) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*8)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 8;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 49) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 3;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case EXTENDED_SYS_STATE:
		if (ind == 0) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, EXTENDED_SYS_STATE_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 1) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, EXTENDED_SYS_STATE_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case STATUSTEXT:
		if (ind == 0) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*8)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 8;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, STATUSTEXT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case UTM_GLOBAL_POSITION:
		if (ind == 68) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			arr.arr[0] = 0x01;
			arr.arr[1] = 0x02;
			arr.arr[2] = 0x03;
			arr.arr[3] = 0x10;
			arr.arr[4] = 0x20;

			// Generate the test files.
			gt(mess, ind, arr, UTM_GLOBAL_POSITION_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case OPEN_DRONE_ID_LOCATION:
		if (ind == 52) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 53) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 2;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 54) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*13)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 13;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 55) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*7)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 7;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 56) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*7)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 7;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 57) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 58) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*15)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 15;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case OPEN_DRONE_ID_SYSTEM:
		if (ind == 50) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 3;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_SYSTEM_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 51) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 2;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_SYSTEM_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	default:
		return;
	}
	
}

/*
 * Convert from 8 bit hexadecimal to integer.
 * Inputs: hex number.
 * Outputs: integer enquvalent.
 */
int toInt8(uint8_t *arr) {
	return (int)arr[0];
}

/*
 * Convert from 16 bit hexadecimal (little endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt16le(uint8_t *arr) {
	return (int)arr[0] + (int)arr[1]*256;
}

/*
 * Convert from 16 bit hexadecimal (big endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt16be(uint8_t *arr) {
	return (int)arr[1] + (int)arr[0]*256;
}

/*
 * Convert from 24 bit hexadecimal (little endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt24le(uint8_t *arr) {
	return (int)arr[0] + (int)arr[1]*256 + (int)arr[2]*4096;
}

/*
 * Convert from 24 bit hexadecimal (big endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt24be(uint8_t *arr) {
	return (int)arr[2] + (int)arr[1]*256 + (int)arr[0]*4096;
}

/*
 * Convert from integer to 24 bit hexadecimal (little endian).
 * Inputs: integer value, array in which to store hex number
 * Outputs: none
 */
void fromInt24le(int num, uint8_t arr[]) {
	arr[0] = num % 256;
	num /= 256;
	arr[1] = num % 256;
	num /= 256;
	arr[2] = num % 256;
	
}

/*
 * Writes a pcap header to a file.
 * Inputs: pcap file data structure, file to which to write.
 * Outputs: none.
 */	
void writeHeader(pcap_t *pcapFile, FILE *ofile) {
	// Variable declarations.
	int i;

	for (i = 0; i < 24; i++) 
		if (fwrite(&(pcapFile->header[i]), sizeof(uint8_t), 1, ofile) != 1)
			printf("Problem writing to file.\n");

}

/*
 * Write raw MAVLink message to a file - just the header and payload.
 * Inputs: message to write, file pointer.
 * Outputs: none.
 */
void writeMavMessageToFile(messageBody_t mess, FILE *ofile) {
	// Variable declarations.
	int i, payload;
	uint8_t buf;
	
	payload = toInt8(&mess.payloadLen);

	for (i = 0; i < payload + 12; i++) {
		if (i == 0) buf = mess.mavCode;
		else if (i == 1) buf =	mess.payloadLen;
		else if (i == 2) buf = mess.incompFlag;
		else if (i == 3) buf = mess.compFlag;
		else if (i == 4) buf = mess.packetSeq;
		else if (i == 5) buf = mess.systemID;
		else if (i == 6) buf = mess.compID;
		else if (i < 10) buf = mess.messageID[i - 7];		
		else if (i < 10 + payload) buf = mess.payload[i - 10];
		else buf = mess.crc[i - 10 - payload];

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Error writing byte %02x to file.\n", buf);
	}

}	
								 
/*
 * Write a MAVLink message to a file including PCAP wrappers.
 * Inputs: message to write, file pointer.
 * Outputs: none.
 */
void writeMessageToFile(message_t mess, FILE *ofile) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;

	len = toInt24le(mess.header.capturedPacketLength) + 16;
	payload = toInt8(&mess.body.payloadLen);
									 
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
		else if (i == 48) buf = mess.body.mavCode;
		else if (i == 49) buf =	mess.body.payloadLen;
		else if (i == 50) buf = mess.body.incompFlag;
		else if (i == 51) buf = mess.body.compFlag;
		else if (i == 52) buf = mess.body.packetSeq;
		else if (i == 53) buf = mess.body.systemID;
		else if (i == 54) buf = mess.body.compID;
		else if (i < 58) buf = mess.body.messageID[i - 55];		
		else if (i < 58 + payload) buf = mess.body.payload[i - 58];
		else buf = mess.body.crc[i - 58 - payload];

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Error writing byte %02x to file.\n", buf);
	}

}

/*
 * Print out a message without it's PCAP wrappers.
 * Inputs: MAVLink message to print
 * Outputs: none.
 */
void printMavMessage(messageBody_t mess) {
// Variable declarations.
	int i, payload;
	uint8_t buf;
	
	payload = toInt8(&mess.payloadLen);

	for (i = 0; i < payload + 12; i++) {
		if (i == 0) buf = mess.mavCode;
		else if (i == 1) buf = mess.payloadLen;
		else if (i == 2) buf = mess.incompFlag;
		else if (i == 3) buf = mess.compFlag;
		else if (i == 4) buf = mess.packetSeq;
		else if (i == 5) buf = mess.systemID;
		else if (i == 6) buf = mess.compID;
		else if (i < 10) buf = mess.messageID[i - 7];		
		else if (i < 10 + payload) buf = mess.payload[i - 10];
		else buf = mess.crc[i - 10 - payload];

		printf("%02x ", buf);
	}

	printf("\n");
}

/*
 * Print out a message including its PCAP wrappers.
 * Inputs: message to print
 * Outputs: none.
 */
void printMessage(message_t mess) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;

	len = toInt24le(mess.header.capturedPacketLength) + 16;
	payload = toInt8(&mess.body.payloadLen);
									 
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
		else if (i == 48) buf = mess.body.mavCode;
		else if (i == 49) buf =	mess.body.payloadLen;
		else if (i == 50) buf = mess.body.incompFlag;
		else if (i == 51) buf = mess.body.compFlag;
		else if (i == 52) buf = mess.body.packetSeq;
		else if (i == 53) buf = mess.body.systemID;
		else if (i == 54) buf = mess.body.compID;
		else if (i < 58) buf = mess.body.messageID[i - 55];		
		else if (i < 58 + payload) buf = mess.body.payload[i - 58];
		else buf = mess.body.crc[i - 58 - payload];

		printf("%02x ", buf);
	}

	printf("\n");
}

/*
 * Writes a specified number of bytes of a MAVLink message to a file.
 * Inputs: message to write, pointer to file to write, number of bytes to write, payload length of the message
 * Outputs: none
 */
void writeMavDiffLenToFile(messageBody_t mess, FILE *fp, int len, int payload) {
	// Variable declarations.
	int i;
	uint8_t buf;

	i = 0;

	// Write specified number of bytes to file.
	while (i < len) {
		if (i == 0) buf = mess.mavCode;
		else if (i == 1) buf =	mess.payloadLen;
		else if (i == 2) buf = mess.incompFlag;
		else if (i == 3) buf = mess.compFlag;
		else if (i == 4) buf = mess.packetSeq;
		else if (i == 5) buf = mess.systemID;
		else if (i == 6) buf = mess.compID;
		else if (i < 10) buf = mess.messageID[i - 7];		
		else if (i < 10 + payload) buf = mess.payload[i - 10];
		else if (i < 12 + payload) buf = mess.crc[i - 10 - payload];
		else buf = (uint8_t)(rand() % 256);
		
		i++;

		if (fwrite(&buf, sizeof(buf), 1, fp) != 1)
			printf("Error writing byte %02x to file.\n", buf);
	}
}

/*
 * Re-inserts the trailing zeros in a MAVLink message to standardise the format for parsing.
 * Inputs: message
 * Outputs: updated message
 */
void untruncate(message_t *mess) {
	// Variable declarations.
	int payload, messageCode, bytesToAdd, len, i, intNum;
	
	messageCode = toInt24le((*mess).body.messageID);
	payload = toInt8(&(*mess).body.payloadLen);

	if (messageCode == HEARTBEAT)
		len = HEARTBEAT_LEN;
	else if (messageCode == SYS_STATUS)
		len = SYS_STATUS_LEN;
	else if (messageCode == PING)
		len = PING_LEN;
	else if (messageCode == LINK_NODE_STATUS)
		len = LINK_NODE_STATUS_LEN;
	else if (messageCode == PARAM_VALUE)
		len = PARAM_VALUE_LEN;
	else if (messageCode == GPS_RAW_INT)
		len = GPS_RAW_INT_LEN;
	else if (messageCode == SCALED_PRESSURE)
		len = SCALED_PRESSURE_LEN;
	else if (messageCode == ATTITUDE)
		len = ATTITUDE_LEN;
	else if (messageCode == ATTITUDE_QUATERNION)
		len = ATTITUDE_QUATERNION_LEN;
	else if (messageCode == LOCAL_POSITION_NED)
		len = LOCAL_POSITION_NED_LEN;
	else if (messageCode == GLOBAL_POSITION_INT)
		len = GLOBAL_POSITION_INT_LEN;
	else if (messageCode == MISSION_CURRENT)
		len = MISSION_CURRENT_LEN;
	else if (messageCode == MISSION_ITEM_REACHED)
		len = MISSION_ITEM_REACHED_LEN;
	else if (messageCode == RC_CHANNELS)
		len = RC_CHANNELS_LEN;
	else if (messageCode == VFR_HUD)
		len = VFR_HUD_LEN;
	else if (messageCode == COMMAND_ACK)
		len = COMMAND_ACK_LEN;
	else if (messageCode == ATTITUDE_TARGET)
		len = ATTITUDE_TARGET_LEN;
	else if (messageCode == POSITION_TARGET_LOCAL_NED)
		len = POSITION_TARGET_LOCAL_NED_LEN;
	else if (messageCode == POSITION_TARGET_GLOBAL_INT)
		len = POSITION_TARGET_GLOBAL_INT_LEN;
	else if (messageCode == RADIO_STATUS)
		len = RADIO_STATUS_LEN;
	else if (messageCode == ALTITUDE)
		len = ALTITUDE_LEN;
	else if (messageCode == BATTERY_STATUS)
		len = BATTERY_STATUS_LEN;
	else if (messageCode == ESTIMATOR_STATUS)
		len = ESTIMATOR_STATUS_LEN;
	else if (messageCode == VIBRATION)
		len = VIBRATION_LEN;
	else if (messageCode == HOME_POSITION)
		len = HOME_POSITION_LEN;
	else if (messageCode == EXTENDED_SYS_STATE)
		len = EXTENDED_SYS_STATE_LEN;
	else if (messageCode == STATUSTEXT)
		len = STATUSTEXT_LEN;
	else if (messageCode == UTM_GLOBAL_POSITION)
		len = UTM_GLOBAL_POSITION_LEN;
	else if (messageCode == TIME_ESTIMATE_TO_TARGET)
		len = TIME_ESTIMATE_TO_TARGET_LEN;
	else if (messageCode == EVENT)
		len = EVENT_LEN;
	else if (messageCode == CURRENT_EVENT_SEQUENCE)
		len = CURRENT_EVENT_SEQUENCE_LEN;
	else if (messageCode == OPEN_DRONE_ID_LOCATION)
		len = OPEN_DRONE_ID_LOCATION_LEN;
	else if (messageCode == OPEN_DRONE_ID_SYSTEM)
		len = OPEN_DRONE_ID_SYSTEM_LEN;
	else
		printf("Invalid message code %d.\n", messageCode);

	if (len != payload) {
		bytesToAdd = len - payload;
		
		intNum = toInt24le((*mess).header.capturedPacketLength) + bytesToAdd;
		for (i = 0; i < 4; i++)
			(*mess).header.capturedPacketLength[i] = (uint8_t)(intNum >> (i * 8));
		
		
		intNum = toInt24le((*mess).header.originalPacketLength) + bytesToAdd;
		for (i = 0; i < 4; i++)
			(*mess).header.originalPacketLength[i] = (uint8_t)(intNum >> (i * 8));
		
		intNum = toInt16be((*mess).header.totalLength) + bytesToAdd;
		for (i = 1; i >= 0; i--)
			(*mess).header.totalLength[i] = (uint8_t)(intNum >> ((i - 1) * 8));
		
		intNum = toInt16be((*mess).header.length) + bytesToAdd;
		for (i = 1; i >= 0; i--)
			(*mess).header.length[i] = (uint8_t)(intNum >> ((i - 1) * 8));

		intNum = toInt8(&(*mess).body.payloadLen) + bytesToAdd;
		(*mess).body.payloadLen = (uint8_t)intNum;

		//INcrease memory for payload
		(*mess).body.payload = (uint8_t *)realloc((*mess).body.payload, sizeof(uint8_t)*(*mess).body.payloadLen);

		// Fill the additional memory with 0s.
		for (i = payload; i < (*mess).body.payloadLen; i++)
			(*mess).body.payload[i] = 0x00;
		
	}
	
}

/*
 * Read a file that contains just MAVLink messagess - no pcap header or pcap wrappers.
 * Inputs: input file
 * Outputs: array of messages; NULL if unsuccessful
 */
mavMessArr_t *readMavFile(FILE *ifile) {
	// Variable declarations.
	mavMessArr_t *messArr;
	int i, payload, numMess, bytesRead;
	messageBody_t mess;
	uint8_t buf;
	
	if ((messArr = (mavMessArr_t *)malloc(sizeof(mavMessArr_t))) == NULL)
		return NULL;

	i = 0;
	payload = 0;
	numMess = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		if (i == 0) mess.mavCode = buf;
		else if (i == 1) {
			mess.payloadLen = buf;

			payload = (int)buf;
			
			if ((mess.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL)
				printf("Memory allocation failed.\n");
		}
		else if (i == 2) mess.incompFlag = buf;
		else if (i == 3) mess.compFlag = buf;
		else if (i == 4) mess.packetSeq = buf;
		else if (i == 5) mess.systemID = buf;
		else if (i == 6) mess.compID = buf;
		else if (i < 10) mess.messageID[i - 7] = buf;		
		else if (i < 10 + payload) mess.payload[i - 10] = buf;
		else mess.crc[i - 10 - payload] = buf;		
		
		i++;

		// If the end of a message has been reached.
		if (i == (12 + payload) && payload != 0) {
			if (numMess == 0) 
				messArr->messages = (messageBody_t *)malloc(sizeof(messageBody_t));
			else 
				messArr->messages = (messageBody_t *)realloc(messArr->messages, sizeof(messageBody_t)*(numMess + 1));

			messArr->messages[numMess] = mess;
					
			// Reset all variables
			i = 0;
			payload = 0;
			numMess++;
		}
	}
	messArr->n = numMess;
	
	return messArr;
}

/*
 * Read a file that contains just messagess - no pcap header.
 * Inputs: input file
 * Outputs: array of messages; NULL if unsuccessful
 */
messArr_t *readFile(FILE *ifile) {
	// variable declarations.
	messArr_t *messArr;
	int i, payload, numMess, bytesRead;
	message_t mess;
	uint8_t buf;

	if ((messArr = (messArr_t *)malloc(sizeof(messArr_t))) == NULL)
		return NULL;

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
		else if (i == 48) mess.body.mavCode = buf;
		else if (i == 49) {
			mess.body.payloadLen = buf;

			payload = (int)buf;
			
			if ((mess.body.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL)
				printf("Memory allocation failed.\n");
		}
		else if (i == 50) mess.body.incompFlag = buf;
		else if (i == 51) mess.body.compFlag = buf;
		else if (i == 52) mess.body.packetSeq = buf;
		else if (i == 53) mess.body.systemID = buf;
		else if (i == 54) mess.body.compID = buf;
		else if (i < 58) mess.body.messageID[i - 55] = buf;		
		else if (i < 58 + payload) mess.body.payload[i - 58] = buf;
		else mess.body.crc[i - 58 - payload] = buf;		
		
		i++;

		// If the end of a message has been reached.
		if (i == (60 + payload) && payload != 0) {
			if (numMess == 0) 
				messArr->messages = (message_t *)malloc(sizeof(message_t));
			else 
				messArr->messages = (message_t *)realloc(messArr->messages, sizeof(message_t)*(numMess + 1));

			messArr->messages[numMess] = mess;
					
			// Reset all variables
			i = 0;
			payload = 0;
			numMess++;
		}
	}
	messArr->n = numMess;
	
	return messArr;
}

/*
 * Read a pcap file (header and all messages.
 * Inputs: input file
 * Outputs: pcap file data structure; NULL if unsuccessful
 */
pcap_t *readPcapFile(FILE *ifile) {
	// variable declarations.
	pcap_t *pcapFile;
	int i, payload, numMess, bytesRead;
	message_t mess;
	uint8_t buf;
	
	if ((pcapFile = (pcap_t *)malloc(sizeof(pcap_t))) == NULL)
		return NULL;

	i = 0;
	
	// Read the first 24 bytes of the file - file header.
	while(i < 24) {
		bytesRead = fread(&buf, sizeof(buf), 1, ifile);

		pcapFile->header[i] = buf;
				
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
		else if (i == 48) mess.body.mavCode = buf;
		else if (i == 49) {
			mess.body.payloadLen = buf;

			payload = (int)buf;
			
			if ((mess.body.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL)
				printf("Memory allocation failed.\n");
		}
		else if (i == 50) mess.body.incompFlag = buf;
		else if (i == 51) mess.body.compFlag = buf;
		else if (i == 52) mess.body.packetSeq = buf;
		else if (i == 53) mess.body.systemID = buf;
		else if (i == 54) mess.body.compID = buf;
		else if (i < 58) mess.body.messageID[i - 55] = buf;		
		else if (i < 58 + payload) mess.body.payload[i - 58] = buf;
		else mess.body.crc[i - 58 - payload] = buf;		
		
		i++;

		// If the end of a message has been reached.
		if (i == (60 + payload) && payload != 0) {
			if (numMess == 0) 
				pcapFile->messages = (message_t *)malloc(sizeof(message_t));
			else 
				pcapFile->messages = (message_t *)realloc(pcapFile->messages, sizeof(message_t)*(numMess + 1));

			pcapFile->messages[numMess] = mess;
					
			// Reset all variables
			i = 0;
			payload = 0;
			numMess++;
		}
	}
	pcapFile->n = numMess;
	
	return pcapFile;
}

/*
 * Free memory from payload, messages array and the structure itself for mavlink messages.
 * Inputs: MAVLink message array structure.
 * Outputs: none.
 */
void freeMemMav(mavMessArr_t *messArr) {
	// Variable declarations.
	int i;
	
	for (i = 0; i < messArr->n; i++)
		free(messArr->messages[i].payload);
	
	free(messArr->messages);
	
	free(messArr);

}

/*
 * Free memory from payload, messages array and the structure itself for a pcap file structure.
 * Inputs: pcap file structure.
 * Outputs: none.
 */
void freeMemPcap(pcap_t *pcapFile) {
	// Variable declarations.
	int i;

	for (i = 0; i < pcapFile->n; i++)
		free(pcapFile->messages[i].body.payload);

	free(pcapFile->messages);

	free(pcapFile);
				 
}

/*
 * Free memory from payload, messages array and the structure itself for a messages array structure.
 * Inputs: message array structure.
 * Outputs: none.
 */
void freeMem(messArr_t *messArr) {
	// Variable declarations.
	int i;

	for (i = 0; i < messArr->n; i++)
		free(messArr->messages[i].body.payload);

	free(messArr->messages);

	free(messArr);

}


/*
 * Generats test cases for a given message id.
 * Inputs: message id
 * Outputs: none
 */
void generateTests(int msgID) {
	// Variable declarations.
	messageBody_t mess;
	int i, j, maxLen;
	uint8_t randomByte;
	char fname[50];
	FILE *fp;
	arrSize_t enumArr;

	// Seed random number generator.
	srand(time(NULL));
	
	// Fill in which fields are enumerated for each message type.
	switch(msgID) {
	case HEARTBEAT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 4;
		enumArr.arr[1] = 5;
		enumArr.arr[2] = 7;

		enumArr.n = 3;

		maxLen = HEARTBEAT_LEN;
		break;
	case SYS_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = SYS_STATUS_LEN;
		break;
	case PING:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = PING_LEN;
		break;
	case LINK_NODE_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = LINK_NODE_STATUS_LEN;
		break;
	case PARAM_VALUE:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 24;

		enumArr.n = 1;

		maxLen = PARAM_VALUE_LEN;
		break;
	case GPS_RAW_INT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 28;

		enumArr.n = 1;

		maxLen = GPS_RAW_INT_LEN;
		break;
	case SCALED_PRESSURE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = SCALED_PRESSURE_LEN;
		break;
	case ATTITUDE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ATTITUDE_LEN;
		break;
	case ATTITUDE_QUATERNION:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ATTITUDE_QUATERNION_LEN;
		break;
	case LOCAL_POSITION_NED:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = LOCAL_POSITION_NED_LEN;
		break;
	case GLOBAL_POSITION_INT:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = GLOBAL_POSITION_INT_LEN;
		break;
	case MISSION_CURRENT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 4;
		enumArr.arr[1] = 5;

		enumArr.n = 2;

		maxLen = MISSION_CURRENT_LEN;
		break;
	case MISSION_ITEM_REACHED:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = MISSION_ITEM_REACHED_LEN;
		break;
	case RC_CHANNELS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = RC_CHANNELS_LEN;;
		break;
	case VFR_HUD:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = VFR_HUD_LEN;
		break;
	case COMMAND_ACK:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 2;

		enumArr.n = 1;

		maxLen = COMMAND_ACK_LEN;
		break;
	case ATTITUDE_TARGET:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ATTITUDE_TARGET_LEN;
		break;
	case POSITION_TARGET_LOCAL_NED:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 50;

		enumArr.n = 1;

		maxLen = POSITION_TARGET_LOCAL_NED_LEN;
		break;
	case POSITION_TARGET_GLOBAL_INT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 50;

		enumArr.n = 1;

		maxLen = POSITION_TARGET_GLOBAL_INT_LEN;
		break;
	case RADIO_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = RADIO_STATUS_LEN;
		break;
	case ALTITUDE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ALTITUDE_LEN;
		break;
	case BATTERY_STATUS:
		if((enumArr.arr = (int *)malloc(sizeof(int)*4)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 33;
		enumArr.arr[1] = 34;
		enumArr.arr[2] = 40;
		enumArr.arr[3] = 49;

		enumArr.n = 4;

		maxLen = BATTERY_STATUS_LEN;
		break;
	case ESTIMATOR_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ESTIMATOR_STATUS_LEN;
		break;
	case VIBRATION:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = VIBRATION_LEN;
		break;
	case HOME_POSITION:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = HOME_POSITION_LEN;
		break;
	case EXTENDED_SYS_STATE:
		if((enumArr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 0;
		enumArr.arr[1] = 1;

		enumArr.n = 2;

		maxLen = EXTENDED_SYS_STATE_LEN;
		break;
	case STATUSTEXT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 0;

		enumArr.n = 1;

		maxLen = STATUSTEXT_LEN;
		break;
	case UTM_GLOBAL_POSITION:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 68;

		enumArr.n = 1;

		maxLen = UTM_GLOBAL_POSITION_LEN;
		break;
	case TIME_ESTIMATE_TO_TARGET:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = TIME_ESTIMATE_TO_TARGET_LEN;
		break;
	case EVENT:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = EVENT_LEN;
		break;
	case CURRENT_EVENT_SEQUENCE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = CURRENT_EVENT_SEQUENCE_LEN;
		break;
	case OPEN_DRONE_ID_LOCATION:
		if((enumArr.arr = (int *)malloc(sizeof(int)*7)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 52;
		enumArr.arr[1] = 53;
		enumArr.arr[2] = 54;
		enumArr.arr[3] = 55;
		enumArr.arr[4] = 56;
		enumArr.arr[5] = 57;
		enumArr.arr[6] = 58;

		enumArr.n = 7;

		maxLen = OPEN_DRONE_ID_LOCATION_LEN;
		break;
	case OPEN_DRONE_ID_SYSTEM:
		if((enumArr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 50;
		enumArr.arr[1] = 51;

		enumArr.n = 2;

		maxLen = OPEN_DRONE_ID_SYSTEM_LEN;
		break;
	default:
		return;
	}

	// Fill header fields for MAVLink 2 tests.
	mess.mavCode = 0xfd;
	mess.incompFlag = 0x00;
	mess.compFlag = 0x00;
	fromInt24le(msgID, mess.messageID);
	
	// Generate a passing test case for each payload length.
	for (i = 1; i <= maxLen; i++) {
		// Set wildcard fields to have random byte values.
		mess.packetSeq = (uint8_t)(rand() % 256);
		mess.systemID = (uint8_t)(rand() % 256);
		mess.compID = (uint8_t)(rand() % 256);
		
		// Set the payload length field.
		mess.payloadLen = (uint8_t)i;
		
		// Allocate memory for payload.
		mess.payload = (uint8_t *)malloc(sizeof(uint8_t) * i);
		
		// Place random bytes into payload (except on ennumerated fields).
		for (j = 0; j < i; j++) {
			// Generate a random byte for wildcard fields.
			randomByte = (uint8_t)(rand() % 256);

			// Differentiate between ennumerated and non-ennumerated fields.
			if (!(arrContains(enumArr, j))) mess.payload[j] = randomByte;
			else mess.payload[j] = fillEnumInRange(msgID, j);
		}

		// Fill in the crc with random bytes.
		mess.crc[0] = (uint8_t)(rand() % 256);
		mess.crc[1] = (uint8_t)(rand() % 256);

		// Create file with name pass.msgID.len where len is the length of the payload being tested.
		sprintf(fname, "./pass.%d.%d", msgID, i);

		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}

		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);
		
		// Write a failing test on the length part of the header field.
		for (j = 0; j <= 255; j++) {
			mess.payloadLen = (uint8_t)j;

			if (j != i)
				sprintf(fname, "./fail.%d.%d.header.payloadLen.%d", msgID, i, j);
			else
				sprintf(fname, "./pass.%d.%d.header.payloadLen.%d", msgID, i, j);
			
			// Open file for writing.
			if ((fp = fopen(fname, "wb")) == NULL) {
				printf("Error opening file %s for writing.\n", fname);
				exit(EXIT_FAILURE);
			}

			// Write message to file.
			writeMavDiffLenToFile(mess, fp, i + 12, i);
		
			fclose(fp);	
		}

		mess.payloadLen = (uint8_t)maxLen;
		
		// Free memory from payload if not full length.
		if (i != maxLen)
			free(mess.payload);
	}

	// Now test a full length message with all fields tested for various bytes.
	for (i = 0; i < maxLen; i++) {		
		// Set wildcard fields to have random byte values.
		mess.packetSeq = (uint8_t)(rand() % 256);
		mess.systemID = (uint8_t)(rand() % 256);
		mess.compID = (uint8_t)(rand() % 256);

		// Deal with non-ennumerated fields.
		if (!(arrContains(enumArr, i))) {
			// Generate tests for each byte value for the given field.
			for (j = 0; j <= 255; j++) {
				mess.payload[i] = (uint8_t)j;

				// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
				sprintf(fname, "./pass.%d.%d.%d.%d", msgID, maxLen, i, j);
				
				// Open file for writing.
				if ((fp = fopen(fname, "wb")) == NULL) {
					printf("Error opening file %s for writing.\n", fname);
					exit(EXIT_FAILURE);
				}
				
				// Write message to file.
				writeMavMessageToFile(mess, fp);
				
				fclose(fp);
			}

			// Fill the field with a random value.
			mess.payload[i] = (uint8_t)(rand() % 266);
		}
		// Deal with enumerated fields producing passing and failing tests.
		else {
			// Generate passing and failing tests for enumerated fields.
			generateEnumTests(msgID, i, mess);

			// Reset the payload of the field to be valid.
			mess.payload[i] = fillEnumInRange(msgID, i);
		}
	}

	// Generate passing and failing tests for MAVLink code field.
	for (j = 0; j <= 255; j++) {
		mess.mavCode = j;
		
		if (j != 0xfd)
			sprintf(fname, "./fail.%d.%d.header.mavCode.%d", msgID, maxLen, j);
		else
			sprintf(fname, "./pass.%d.%d.header.mavCode.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.mavCode = 0xfd;

	// Generate passing and failing tests for incompatibility flag field.
	for (j = 0; j <= 255; j++) {
		mess.incompFlag = j;
		
		if (j != 0x00)
			sprintf(fname, "./fail.%d.%d.header.incompFlag.%d", msgID, maxLen, j);
		else
			sprintf(fname, "./pass.%d.%d.header.incompFlag.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.incompFlag = 0x00;

	// Generate passing and failing tests for compatibility flag field.
	for (j = 0; j <= 255; j++) {
		mess.compFlag = j;
		
		if (j != 0x00)
			sprintf(fname, "./fail.%d.%d.header.compFlag.%d", msgID, maxLen, j);
		else
			sprintf(fname, "./pass.%d.%d.header.compFlag.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.compFlag = 0x00;

	// Write a failing test with an extra byte.
	sprintf(fname, "./fail.%d.%d.extraByte", msgID, maxLen);
	
	// Open file for writing.
	if ((fp = fopen(fname, "wb")) == NULL) {
		printf("Error opening file %s for writing.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	// Write message to file wuth an extra byte.
	writeMavDiffLenToFile(mess, fp, i + 13, i);
	
	fclose(fp);

	// Write a failing test with a byte too few.
	sprintf(fname, "./fail.%d.%d.oneLessByte", msgID, maxLen);
	
	// Open file for writing.
	if ((fp = fopen(fname, "wb")) == NULL) {
		printf("Error opening file %s for writing.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	// Write message to file with a byte too few.
	writeMavDiffLenToFile(mess, fp, i + 11, i);
	
	fclose(fp);

	// Generate passing system id tests.
	for (j = 0; j <= 255; j++) {
		mess.systemID = j;
		
		sprintf(fname, "./pass.%d.%d.header.systemID.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.systemID = (uint8_t)(rand() % 256);

		// Generate passing component id tests.
	for (j = 0; j <= 255; j++) {
		mess.compID = j;
		
		sprintf(fname, "./pass.%d.%d.header.componentID.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.compID = (uint8_t)(rand() % 256);

	// Generate passing packet sequence tests.
	for (j = 0; j <= 255; j++) {
		mess.packetSeq = j;
		
		sprintf(fname, "./pass.%d.%d.header.packetSequence.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.packetSeq = (uint8_t)(rand() % 256);;

	// Generate passing crc field tests.
	for (j = 0; j <= 255; j++) {
		mess.crc[0] = j;
		
		sprintf(fname, "./pass.%d.%d.crc0.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.crc[0] = (uint8_t)(rand() % 256);;

	for (j = 0; j <= 255; j++) {
		mess.crc[1] = j;
		
		sprintf(fname, "./pass.%d.%d.crc1.%d", msgID, maxLen, j);
		
		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}
		
		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);	
	}
	mess.crc[1] = (uint8_t)(rand() % 256);;
	
	free(mess.payload);
	
	if (enumArr.arr != NULL)
		free(enumArr.arr);
}
