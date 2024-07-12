/* 
 * extractdata.c --- extracts all messages of a given code or codes from a .pcap file and writen them to a new file; assumes input file has already been cleaned with cleandata.c
 * 
 * Author: Joshua M. Meise
 * Created: 06-21-2024
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

// Define the message body wiht the various fields.
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
	uint8_t *header;
	messageBody_t body;
} message_t;

/* 
 * Converts a 32 bit hexadecimal number (stored in an array to an integer.
 * Inputs: the start index of the array, how many bytes are in the hex representation of the number
 * Outputs: the integer value of the length of the message
 */

static int toInt(uint8_t *arr) {
	return (int)arr[0] + (int)arr[1]*256 + (int)arr[2]*4096;
}

/* 
 * For debugging; prints the message to the screen.
 * Inputs: message to be printed, number of bytes in the message.
 * Outputs: none.
 */
static void printMessage(message_t mess, int len) {
	int i, payload;

	payload = mess.body.payloadLen;
	
	printf("Message: ");
	for (i = 0; i < len; i++) {
		if (i < MAVLEN) printf("%02x ", mess.header[i]);
		else if (i == MAVLEN) printf("%02x ", mess.body.mavCode);
		else if (i == MAVLEN + 1) printf("%02x ", mess.body.payloadLen);
		else if (i == MAVLEN + 2) printf("%02x ", mess.body.incompFlag);
		else if (i == MAVLEN + 3) printf("%02x ", mess.body.compFlag);
		else if (i == MAVLEN + 4) printf("%02x ", mess.body.packetSeq);
		else if (i == MAVLEN + 5) printf("%02x ", mess.body.systemID);
		else if (i == MAVLEN + 6) printf("%02x ", mess.body.compID);
		else if (i < MAVLEN + 10) printf("%02x ", mess.body.messageID[i - MAVLEN - 7]);
		else if (i < MAVLEN + payload + 10) printf("%02x ", mess.body.payload[i - MAVLEN - 10]);
		else printf("%02x ", mess.body.crc[i - MAVLEN - payload - 10]);
	}

	printf("\n");
}


/*
 * Write a message to a file in binary.
 * Inputs: message to be written, length of message (number of bytes), stream to write to.
 * Outputs: none.
 */
static void writeMessageToFile(message_t mess, int len, FILE *ofile) {
	int i, payload;
	uint8_t buf;

	payload = mess.body.payloadLen;
	
	for (i = 0; i < len; i++) {
		if (i < MAVLEN) buf = mess.header[i];
		else if (i == MAVLEN) buf = mess.body.mavCode;
		else if (i == MAVLEN + 1) buf = mess.body.payloadLen;
		else if (i == MAVLEN + 2) buf = mess.body.incompFlag;
		else if (i == MAVLEN + 3) buf = mess.body.compFlag;
		else if (i == MAVLEN + 4) buf = mess.body.packetSeq;
		else if (i == MAVLEN + 5) buf = mess.body.systemID;
		else if (i == MAVLEN + 6) buf = mess.body.compID;
		else if (i < MAVLEN + 10) buf = mess.body.messageID[i - MAVLEN - 7];
		else if (i < MAVLEN + payload + 10) buf = mess.body.payload[i - MAVLEN - 10];
		else buf = mess.body.crc[i - MAVLEN - payload - 10];

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Message write failed.\n");
	}
}

int main(int argc, char **argv) {
	// Variable declarations
	FILE *ifile, *ofile;
	uint8_t buf;
	int bytesRead, i, *codesArr, numCodes, payload;
	message_t mess;

	// Check arguments
	if (argc < 4) {
    printf("usage: extractdata inputFile outputFile messageCodes\n");
    exit(EXIT_FAILURE);
	}

	// Check that provided file exists by opening for binary reading
	if ((ifile = fopen(argv[1], "rb")) == NULL) {
		printf("File does not exist\n");
		exit(EXIT_FAILURE);
	}

	// See how many codes the user entered.
	numCodes = argc - 3;
	
	// ensure that codes are psoitive integers.
	for (i = 0; i < numCodes; i++) {
		if (atoi(argv[i + 3]) < 0) {
			printf("usage: extractdata inputFile outputFile messageCodes\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Allocate space for array of codes to be extracted.
	if ((codesArr = (int *)malloc(sizeof(int)*(numCodes))) == NULL)
		printf("Memory allocation failed.\n");

	// Fill array with user's requested extraction codes.
	for (i = 0; i < numCodes; i++)
		codesArr[i] = atoi(argv[3 + i]);
	
	// Open output file for binary writing
	if ((ofile = fopen(argv[2], "wb")) == NULL)
		printf("File not opened successfully\n");
	
	i = 0;

	// Read the first 24 bytes of the .pcap file into the pcap header and write to the output file
	while(i < 24) {
		bytesRead = fread(&buf, sizeof(buf), 1, ifile);
		i++;
		
		//		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
		//			printf("Not successfully written");
		
	}

	i = 0;
	payload = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		// Allocate space for the header field.
		if (i == 0)
			if ((mess.header = (uint8_t *)malloc(sizeof(uint8_t)*MAVLEN)) == NULL)
				printf("Memory allocation unsuccessful.\n");
		
		// Read the first 48 bytes into the header field
		if (i < MAVLEN) mess.header[i] = buf;
		
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
			for (i = 0; i < numCodes; i++) {
				if (toInt(mess.body.messageID) == codesArr[i]) {
					printMessage(mess, MAVLEN + payload + 12);
					writeMessageToFile(mess, MAVLEN + payload + 12, ofile);
				}
			}
			
			//			else if (mess.body[0] == 0xfd && argc == 4 && currMessage == messageNum)
			//writeMessageToFile(mess, len + MAVLEN, ofile);

			// Reset all variables
			i = 0;
			payload = 0;
			
			// Free memory.
			free(mess.header);
			free(mess.body.payload);
		}

	}

	free(codesArr);
	fclose(ifile);
	fclose(ofile);
	exit(EXIT_SUCCESS);
}
