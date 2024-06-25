/* 
 * countcodes.c --- counts the number of unique mavlink messages in a file; expects cleaned file as input
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
 * Inserts an element into the array of codes.
 * Inputs: pointer to the array into which to insert, pointer to number of elements in array, code to insert.
 * Outputs: None.
 */
static int insert(int **arr, int num, int code) {
	printf("IN FUNC\n");
	// Create new array if array does not exist, otherwise, just grow it by 1.
	if (num == 0) {
		//		if ((*arr = (int *)malloc(sizeof(int))) == NULL)
		//printf("Memory allocation failed.\n");

		//		(*arr)[*num] = code;
		num++;
	} else {
		//		if ((*arr = (int *)realloc(*arr, *num + 1)) == NULL)
		//			printf("Memory allocation failed.\n");

		//(*arr)[*num] = code;
		num++;
	}

	return num;
}

/* 
 * Checks if array of codes contains an element.
 * Inputs: array of codes, length of array of codes, code to search for.
 * Outputs: true if found, false otherwise.
 */
bool contains(int *codesArr, int num, int searchCode) {
	// Variable declarations.
	int i;
	
	for (i = 0; i < num; i++)
		if (codesArr[i] == searchCode)
			return true;

	return false;
}

int main(int argc, char **argv) {
	// Variable declarations
	FILE *ifile;
	uint8_t buf;
	int bytesRead, i, *codesArr, numCodes, payload;
	message_t mess;

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
	numCodes = 0;
	
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
			// If array does not yet exist create one and insert code.
			//			if (numCodes == 0)
				//				numCodes = insert(&codesArr, numCodes, toInt(mess.body.messageID));
			// Otherwise search array and insert if it does not contain code.
			//	else if (contains(codesArr, numCodes, toInt(mess.body.messageID)) == false)
			//				numCodes = insert(&codesArr, numCodes, toInt(mess.body.messageID));
			
			// Reset all variables
			i = 0;
			payload = 0;
			
			// Free memory.
			free(mess.header);
			free(mess.body.payload);
		}
		
	}

	printf("unique codes = %d\n", numCodes);
	
	//	free(codesArr);
	fclose(ifile);
	exit(EXIT_SUCCESS);
}
