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
	uint8_t *data;
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

	for (i = 0; i < mess->pph.pktDataLen + 1; i++) {
		buf = mess->data[i];
		
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
 * Callback function to print message.
 * Inputs: pointer to message to print
 * Outputs: none
 */
static void printFuncSpp(void *data) {
	// Variable declarations.
	sppMessage_t *mess;

	// Coerce.
	mess =(sppMessage_t *)data;

	// Write message to file.
	printSppMessage(mess);

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
 * Print SPP messgaes.
 * Inputs: SPP data structure
 * Outputs: none
 */
void printSpp(spp_t *spp) {
	// Variable declarations.
	sppPrivate_t *sppP;

	// Check arguments.
	if (spp == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return;
	}

	// Coerce into valid datatype.
	sppP = (sppPrivate_t *)spp;

	// Write each message to a file.
	vectorApply(sppP->messages, printFuncSpp);
}

/*
 * Reads SPP message from a binary file.
 * Inputs: File from which to read.
 * Outputs: SPP data structure, NULL if unsuccessful
 */
spp_t *readSPPFile(FILE *ifile) {
	// Variable declarations.
	sppPrivate_t *spp;
	uint8_t byte;
	int i, cnt;
	uint16_t len;
	
	// Check arguments.
	if (ifile == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}
	
	// Allocate memory for data structure.
	if ((spp = (sppPrivate_t *)malloc(sizeof(sppPrivate_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}
	
	// Initialise messages vector.
	if ((spp->messages = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialization failed.\n");
		return NULL;
	}
	
	i = 0;
	cnt = 0;
	
	// Read all bytes from file.
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 0) {
			mess->pph.versionNumber = ((uint16_t)byte >> 5);
			mess->pph.pktType = ((uint16_t)byte >> 4);
			mess->pph.secHdrFlag = ((uint16_t)byte >> 3);
			mess->pph.apid = ((uint16_t)byte & 0x07) << 8;
		} else if (i == 1) {
			mess->pph.apid |= (uint16_t)byte;
		} else if (i == 2) {
			mess->pph.seqFlags = (uint16_t)byte >> 6;
			mess->pph.pktSeqOrPn = ((uint16_t)byte & 0x3F) << 8;
		} else if (i == 3) {
			mess->pph.pktSeqOrPn |= (uint16_t)byte;
		} else if (i == 4) {
			mess->pph.pktDataLen = (uint16_t)byte << 8;
		} else if (i == 5) {
			mess->pph.pktDataLen |= (uint16_t)byte;

			if ((mess->data = (uint8_t *)malloc(sizeof(uint8_t)*mess->pph.pktDataLen))== NULL) {
				fprintf(stderr, "Memoryallocation failed.\n");
				return NULL;
			}
		} else {
			mess->data[cnt++] = byte;
		}

		if (cnt == mess->pph.pktDataLen + 1) {
			cnt = 0;
			i = 0;
		} else
			i++;
	}

	return (spp_t *)spp;
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
	int i;
	uint16_t old;
	
	// Seed random number generator.
	srand(time(NULL));

	// Set the various fields.
	mess.pph.versionNumber = 0;
	mess.pph.pktType = 1;
	mess.pph.secHdrFlag = 0;
	mess.pph.apid = (rand() % (1 << 11));
	mess.pph.seqFlags = 0;
	mess.pph.pktSeqOrPn = (rand() % (1 << 14));
	mess.pph.pktDataLen = (rand() % (1 << 16));
	
	// First passing test.
	sprintf(fname, "./pass.%d", passSeed++);
	
	// Open file.
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}

	// Write test to file.
	writeSppMessageToFile(&mess, fp);

	// Close file.
	fclose(fp);

	// Save the old value of version number.
	old = mess.pph.versionNumber;
	
	// Generate failing test cases for all values in version number.
	for (i = 1; i <= 7; i++) {
		mess.pph.versionNumber = i;

		// Generate failing test.
		sprintf(fname, "./fail.%d", failSeed++);
		
		// Open file.
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			exit(EXIT_FAILURE);
		}
		
		// Write test to file.
		writeSppMessageToFile(&mess, fp);
		
		// Close file.
		fclose(fp);
	}

	// Revert to old value of version number.
	mess.pph.versionNumber = old;

	// Save the old value of packet type.
	old = mess.pph.pktType;
	
	// Generate passing test case for other value of packet type..
	mess.pph.pktType = 0;
	
	// Generate passing test.
	sprintf(fname, "./pass.%d", passSeed++);
		
	// Open file.
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	
	// Write test to file.
	writeSppMessageToFile(&mess, fp);
	
	// Close file.
	fclose(fp);

	// Revert to old value of pckaet type.
	mess.pph.pktType = old;

	// Generate further passing test on APID.
	old = mess.pph.apid;

	mess.pph.apid = (rand() % (1 << 11));
	
	// Generate passing test.
	sprintf(fname, "./pass.%d", passSeed++);
	
	// Open file.
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	
	// Write test to file.
	writeSppMessageToFile(&mess, fp);
	
	// Close file.
	fclose(fp);

	mess.pph.apid = old;

	// Generate further passing tests on sequence flags.
	old = mess.pph.seqFlags;

	for (i = 1; i <= 3; i++) {
		mess.pph.seqFlags = i;

		// Generate passing test.
		sprintf(fname, "./pass.%d", passSeed++);
		
		// Open file.
		if ((fp = fopen(fname, "wb")) == NULL) {
			fprintf(stderr, "Failed to open file.\n");
			exit(EXIT_FAILURE);
		}
		
		// Write test to file.
		writeSppMessageToFile(&mess, fp);
		
		// Close file.
		fclose(fp);
	}

	mess.pph.seqFlags = old;

	// Generate further passing test on pcaket sequence numbe or packet name.
	old = mess.pph.pktSeqOrPn;

	mess.pph.pktSeqOrPn = (rand() % (1 << 14));
	
	// Generate passing test.
	sprintf(fname, "./pass.%d", passSeed++);
	
	// Open file.
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	
	// Write test to file.
	writeSppMessageToFile(&mess, fp);
	
	// Close file.
	fclose(fp);

	mess.pph.pktSeqOrPn = old;

	// Generate further passing test on packet data length.
	old = mess.pph.pktDataLen;

	mess.pph.pktDataLen = (rand() % (1 << 16));
	
	// Generate passing test.
	sprintf(fname, "./pass.%d", passSeed++);
	
	// Open file.
	if ((fp = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	
	// Write test to file.
	writeSppMessageToFile(&mess, fp);
	
	// Close file.
	fclose(fp);

	mess.pph.pktDataLen = old;

}
