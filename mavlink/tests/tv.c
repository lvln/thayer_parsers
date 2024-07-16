/* 
 * tv.c -- extracts a single message from a file containign data from drone flight
 *         writes each message to a file individually
 *         iterates over all non-constant data fields and changes values generating passing tests
 *         iterates over all constant fieldsa dn changes values generating failing tests
 * 
 * Author: Joshua M. Meise
 * Created: 07-16-2024
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>
#include <stdbool.h>

typedef struct codeDone {
	int code;
	bool done;
} codeDone_t;

typedef struct array {
	codeDone_t *codeDone;
	int n;
} array_t;

static void writeEditedFile(message_t *mess, FILE *fp, int len, int payload) {
	int i;
	uint8_t buf;

	i = 0;
	
	while (i < len) {
		if (i < 4) buf = mess->header.timeS[i];
		else if (i < 8) buf = mess->header.timeUNS[i - 4];
		else if (i < 12) buf = mess->header.capturedPacketLength[i - 8];
		else if (i < 16) buf = mess->header.originalPacketLength[i - 12];
		else if (i < 20) buf = mess->header.messageFam[i - 16];
		else if (i == 20) buf = mess->header.iphl;
		else if (i == 21) buf = mess->header.dsf;
		else if (i < 24) buf = mess->header.totalLength[i - 22];
		else if (i < 26) buf = mess->header.id[i - 24];
		else if (i < 28) buf = mess->header.ffOffset[i - 26];
		else if (i == 28) buf = mess->header.ttl;
		else if (i == 29) buf = mess->header.protocol;
		else if (i < 32) buf = mess->header.headerChecksum[i - 30];
		else if (i < 36) buf = mess->header.sourceAddr[i - 32];
		else if (i < 40) buf = mess->header.destAddr[i - 36];
		else if (i < 42) buf = mess->header.sourcePort[i - 40];
		else if (i < 44) buf = mess->header.destPort[i - 42];
		else if (i < 46) buf = mess->header.length[i - 44];
		else if (i < 48) buf = mess->header.checkSum[i - 46];
		else if (i == 48) buf = mess->body.mavCode;
		else if (i == 49) buf =	mess->body.payloadLen;
		else if (i == 50) buf = mess->body.incompFlag;
		else if (i == 51) buf = mess->body.compFlag;
		else if (i == 52) buf = mess->body.packetSeq;
		else if (i == 53) buf = mess->body.systemID;
		else if (i == 54) buf = mess->body.compID;
		else if (i < 58) buf = mess->body.messageID[i - 55];		
		else if (i < 58 + payload) buf = mess->body.payload[i - 58];
		else if (i < 60 + payload) buf = mess->body.crc[i - 58 - payload];
		else buf = 0x21;
		
		i++;

		fwrite(&buf, sizeof(buf), 1, fp);

	}
}

static bool contains(array_t *arr, int comp) {
	// Varaible declarations.
	int i;

	for (i = 0; i < arr->n; i++) {
		if (arr->codeDone[i].code == comp)
			return true;
	}

	return false;
}

static array_t *countCodes(pcap_t *pcapFile) {
	// Variable declarations.
	int i;
	array_t *arr;

	if ((arr = (array_t *)malloc(sizeof(array_t))) == NULL)
		printf("Problem with memory allocation.\n");

	arr->codeDone = NULL;
	arr->n = 0;
	
	for (i = 0; i < pcapFile->n; i++) {
		// If the first code being inserted then allocate an array for it an insert the code. Also increment the number of elements in the array.
		if (arr->n == 0) {
			arr->codeDone = (codeDone_t *)malloc(sizeof(codeDone_t));
			arr->codeDone[arr->n].code = toInt24le(pcapFile->messages[i].body.messageID);
			arr->codeDone[arr->n].done = false;
			(arr->n)++;
		}
		// If the code is not contained in the array, add it into the array by reallocating space.
		else if (!(contains(arr,toInt24le(pcapFile->messages[i].body.messageID)))) {
			arr->codeDone = (codeDone_t *)realloc(arr->codeDone, sizeof(codeDone_t)*(arr->n + 1));
			arr->codeDone[arr->n].code = toInt24le(pcapFile->messages[i].body.messageID);
			arr->codeDone[arr->n].done = false;
			(arr->n)++;
		}
	}
				
	return arr;		
}

static void freeArr(array_t *arr) {
	// Free the embedded array.
	free(arr->codeDone);

	// Free the structure itself.
	free(arr);
}

int main(int argc, char **argv) {
	// Variable declarations.
	pcap_t *pcapFile;
	FILE *fp;
	array_t *arr;
	int i, j, currCode;
	bool done;
	char fname[20];
	messArr_t *messArr;
	uint8_t old;
	
	// Check number of arguments.
	if (argc != 2) {
		printf("usage: ./extractone inputFile\n");
		exit(EXIT_FAILURE);
	}

	// Make sure file can open.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("usage: ./extractone inputFile\n");
		exit(EXIT_FAILURE);
	}

	// Read file into data structure.
	pcapFile = readPcapFile(fp);

	fclose(fp);

	// COunt codes and fill array of codes.
	arr = countCodes(pcapFile);

	// Go through all messages in file.
	for (i = 0; i < pcapFile->n; i++) {
		//Extract current message's code.
		currCode = toInt24le(pcapFile->messages[i].body.messageID);
		done = true;

		// Check if the code has already been processed.
		for (j = 0; j < arr->n; j++) {
			if (arr->codeDone[j].code == currCode && arr->codeDone[j].done == false) {
				done = false;
				arr->codeDone[j].done = true;
			}
		}

		// If the code has not been processed, write that code's message to a file.
		if (done == false) {
			sprintf(fname, "./pass.%d", currCode);

			if ((fp = fopen(fname, "wb")) == NULL)
				printf("Error opening file for writing.\n");

			writeToFile(pcapFile->messages[i], fp);

			fclose(fp);
		}
		
	}

	// Loop through all passing test files.
	for (i = 0; i < arr->n; i++) {
		// Open the corresponding file and read the message in.
		sprintf(fname, "./pass.%d", arr->codeDone[i].code);

		if ((fp = fopen(fname, "rb")) == NULL)
			printf("Error opening file.\n");
		
		messArr = readFile(fp);

		fclose(fp);

		// Loop through all fields.
		for (j = 0; j < messArr->messages[0].body.payloadLen; j++) {
			// Create a new file name.
			sprintf(fname, "./pass.%d.%d", arr->codeDone[i].code, j);

			if ((fp = fopen(fname, "wb")) == NULL)
				printf("Error opening file.\n");

			// Change the field in the array.
			old = messArr->messages[0].body.payload[j];
			messArr->messages[0].body.payload[j] = old + 0x01;

			// Write the updated file.
			writeToFile(messArr->messages[0], fp);
			
			// Revert the field.
			messArr->messages[0].body.payload[j] = old;
			
		}

		// Generate failure tests by changing fixed fields.
		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 0);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
	
		old = messArr->messages[0].body.mavCode;
		messArr->messages[0].body.mavCode = old + 0x01;
	
		writeToFile(messArr->messages[0], fp);
		
		fclose(fp);
		
		messArr->messages[0].body.mavCode = old;

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 1);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[0].body.messageID[0];
		messArr->messages[0].body.messageID[0] = old + 0x10;
	
		writeToFile(messArr->messages[0], fp);
		
		fclose(fp);
		
		messArr->messages[0].body.messageID[0] = old;

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 2);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[0].body.payloadLen;
		messArr->messages[0].body.payloadLen = old + 0x01;

		writeEditedFile(&(messArr->messages[0]), fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16, toInt16be(messArr->messages[0].header.totalLength) - 40);
		
		fclose(fp);
		
		messArr->messages[0].body.payloadLen = old;

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 3);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		writeEditedFile(&(messArr->messages[0]), fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16 - 1, toInt16be(messArr->messages[0].header.totalLength) - 40);
	
		fclose(fp);

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 4);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);

		writeEditedFile(&(messArr->messages[0]), fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16 + 1, toInt16be(messArr->messages[0].header.totalLength) - 40);
	
		fclose(fp);

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 5);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[0].header.messageFam[0];
		messArr->messages[0].header.messageFam[0] = old + 0x01;
	
		writeToFile(messArr->messages[0], fp);
	
		fclose(fp);
		
		messArr->messages[0].header.messageFam[0] = old;

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 6);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[0].header.iphl;
		messArr->messages[0].header.iphl = old + 0x01;

		writeToFile(messArr->messages[0], fp);
		fclose(fp);
		
		messArr->messages[0].header.iphl = old;

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 7);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);

		old = messArr->messages[0].header.ttl;
		messArr->messages[0].header.ttl = old + 0x01;
		
		writeToFile(messArr->messages[0], fp);
		
		fclose(fp);
		
		messArr->messages[0].header.ttl = old;

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 8);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[0].header.sourcePort[0];
		messArr->messages[0].header.sourcePort[0] = old + 0x01;
		
		writeToFile(messArr->messages[0], fp);
	
		fclose(fp);
		
		messArr->messages[0].header.sourcePort[0] = old;

		sprintf(fname, "./fail.%d.%d", arr->codeDone[i].code, 9);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[0].header.destPort[0];
		messArr->messages[0].header.destPort[0] = old + 0x01;
		
		writeToFile(messArr->messages[0], fp);
		
		fclose(fp);

		messArr->messages[0].header.destPort[0] = old;

	}
	
	freeArr(arr);
	
	freeMemPcap(pcapFile);

	exit(EXIT_SUCCESS);
}
