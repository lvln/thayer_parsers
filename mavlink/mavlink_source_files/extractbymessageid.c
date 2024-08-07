/* 
 * extractbymessageid.c --- extracts all messages of a given message id from PCAP file and and outputs the MAVLink portion to a file
 * 
 * Author: Joshua M. Meise
 * Created: 07-25-2024
 * Version: 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>
#include <vector.h>
#include <stdbool.h>

/*
 * Checks to see if 2 integers are the same
 * Input: numbers to search for
 * Output: true if same, false otherwise
 */
static bool compareInt(void *vp1, void *vp2) {
	// Variable declarations.
	int *i, *j;

	// Coerce values.
	i = (int *)vp1;
	j = (int *)vp2;

	if (*i == *j) return true;
	else return false;
}

int main(int argc, char **argv) {
	// Variable declarations.
	FILE *fp;
	int i, num, *ins, id;
	pcap_t *pcap;
	vector_t *vec;
	
	// Check number of arguments.
	if (argc < 4) {
		printf("usage: extractbymessageid inputFile[.pcap] outputFile[.mav] messageID[s]\n");
		exit(EXIT_FAILURE);
	}

	// Make sure file exists and can open.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("usage: extractbymessageid inputFile[.pcap] outputFile[.mav] messageID[s]\n");
		exit(EXIT_FAILURE);
	}

	// Allocate memory.
	if ((vec = vectorInit()) == NULL)
		exit(EXIT_FAILURE);
	
	// Extract which message ids are to be extacted.
	for (i = 0; i < argc - 3; i++) {
		if ((num = atoi(argv[i + 3])) < 1) {
			printf("Neagtive index.\n");
			exit(EXIT_FAILURE);
		}

		// Allocate memory for data to be inserted to vector.
		if ((ins = (int *)malloc(sizeof(int))) == NULL) {
			printf("Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}

		*ins = num;
		
		// Insert to vector.
		vectorInsertBack(vec, (void *)ins);
	}
	
	// Read in pcap file
	pcap = readFilePcap(fp);

	fclose(fp);

	// Open file for reading.
	if ((fp = fopen(argv[2], "wb")) == NULL) {
		printf("usage: extractbymessageid inputFile[.pcap] outputFile[.mav] messageID[s]\n");
		exit(EXIT_FAILURE);
	}

	// Write message to file if number was input
	for (i = 0; i < pcap->n; i++) {
		id = toInt24le(pcap->messages[i].body.messageID);
		if (vectorContains(vec, compareInt, (void *)&(id)))
			writeMessageToFileMav(pcap->messages[i].body, fp);
	}

	fclose(fp);

	freeMemPcap(pcap);
	vectorApply(vec, free);
	vectorFree(vec);
	exit(EXIT_SUCCESS);
}
