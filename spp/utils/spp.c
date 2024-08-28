/* 
 * spp.c --- module implementing SPP data procesing and test generation functions
 * 
 * Author: Joshua M. Meise
 * Created: 08-26-2024
 * Version: 1.0
 * 
 */

#include <spp.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector.h>
#include <time.h>

/*
 * Data structure defining packet primary header format.
 */
typedef struct packetPrimaryHeader {
	uint16_t versionNumber: 3;
	uint16_t pktType: 1;
	uint16_t secHdrFlag: 1;
	uint16_t apid: 11;
	uint16_t seqFlags: 2;
	uint16_t pktSeqOrPn: 14;
	uint16_t pktDataLen;
} packetPrimaryHeader_t;

/*
 * Data structure defining the SPP message.
 */
typedef struct sppMessage {
	packetPrimaryHeader_t pph;
} sppMessage_t;

/*
 * Data structure to store private representation of SPP.
 */
typedef struct sppPrivate {
	vector_t *messages;
} sppPrivate_t;

static FILE *ofile;

/*
 * Given the number of bits, prints the binary number.
 * Inputs: number to print, nunber of bits
 * Outputs: none
 */
static void printBinary(uint16_t num, int bits) {
	// Varibale declarations.
	int i;

	for (i = 0; i < bits; i++)
		printf("%d", ((num >> (bits - 1 - i)) & 1));

}

/*
 * Print an idividual SPP message.
 * Inputs: message to print
 * Outputs: none
 */
static void printSppMessage(sppMessage_t *mess) {
	// Check argument.
	if (mess == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	printf("Packet version number: ");
	printBinary(mess->pph.versionNumber, 3);
	printf("\n");

	printf("Packet type: ");
	printBinary(mess->pph.pktType, 1);
	printf("\n");

	printf("Secondary header flag: ");
	printBinary(mess->pph.secHdrFlag, 1);
	printf("\n");

	printf("Application process ID: ");
	printBinary(mess->pph.apid, 11);
	printf("\n");

	printf("Sequence flags: ");
	printBinary(mess->pph.seqFlags, 2);
	printf("\n");

	printf("Packet sequence count or packet name: ");
	printBinary(mess->pph.pktSeqOrPn, 14);
	printf("\n");

	printf("Packet data length: ");
	printBinary(mess->pph.pktDataLen, 16);
	printf("\n");
}

/*
 * Write an individual SPP message to a given file.
 * Inputs: message to write, file to which to write
 * Outputs: none
 */
static void writeSppMessageToFile(sppMessage_t *mess, FILE *fp) {
	// Variable declarations.
	int i;
	uint8_t buf, byteArray[6];
	
	// Check arguments.
	if (mess == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}

	// First initialise array of bytes.
	for (i = 0; i < 6; i++)
		byteArray[i] = 0;
	
	// Write bit fields to array of bytes.
	byteArray[0] = (mess->pph.versionNumber << 5) | (mess->pph.pktType << 4) | (mess->pph.secHdrFlag << 3) | (mess->pph.apid >> 8);
	byteArray[1] = mess->pph.apid & 0xFF;
	byteArray[2] = (mess->pph.seqFlags << 6) | (mess->pph.pktSeqOrPn >> 8);
	byteArray[3] = mess->pph.pktSeqOrPn & 0xFF;
	byteArray[4] = (mess->pph.pktDataLen >> 8) & 0xFF;
	byteArray[5] = mess->pph.pktDataLen & 0xFF;
	
	for (i = 0; i < 6; i++) {
		buf = byteArray[i];
		
		if (fwrite(&buf, sizeof(uint8_t), 1, fp) != 1)
			fprintf(stderr, "Error writing byte %02x to file.", buf);
	}
	
}

/*
 * Callback function to write emssage to file.
 * Inputs: pointer to message to write.
 * Outputs: none
 */
static void writeFuncSpp(void *data) {
	// Variable declarations.
	sppMessage_t *mess;

	// Coerce.
	mess =(sppMessage_t *)data;

	// Write message to file.
	writeSppMessageToFile(mess, ofile);

}

/*
 * Write SPP test to file.
 * Inputs: SPP data structure, file to which to write
 * Outputs: none
 */
void writeSppToFile(spp_t *spp, FILE *fp) {
	// Variable declarations.
	sppPrivate_t *sppP;

	// Check arguments.
	if (spp == NULL || fp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}

	// Coerce into valid datatype.
	sppP = (sppPrivate_t *)spp;

	// Set output file global variable.
	ofile = fp;
	
	// Write each message to a file.
	vectorApply(sppP->messages, writeFuncSpp);
}

/*
 * Generate tests for SPP messages.
 * Inputs: pass seed, fail seed
 * Outputs: none
 */
void generateTests(int passSeed, int failSeed) {
	// Variable declarations.
	sppMessage_t mess;
	FILE *fp;
	char fname[50];

	// Seed random number generator.
	srand(time(NULL));

	// Set the various fields.
	mess.pph.versionNumber = 0;
	mess.pph.pktType = 1;
	mess.pph.secHdrFlag = 1;
	mess.pph.apid = (rand() % (1 << 11));
	mess.pph.seqFlags = 3;
	mess.pph.pktSeqOrPn = (rand() % (1 << 14));
	mess.pph.pktDataLen = (rand() % (1 << 16));
	
	// First passing test.
	sprintf(fname, "./pass.%d", passSeed++);
	
	// Open file.
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}

	printSppMessage(&mess);

	// Write test to file.
	writeSppMessageToFile(&mess, fp);

	// Close file.
	fclose(fp);

	// Generate fialing test cases for all values in version number.
	//	for (i = 0; i < 
	
}
