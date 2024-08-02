/* 
 * extractbymessagenumber.c --- extracts a given message/s (1 indexed sequence) from a PCAP (default) or .mav file and outputs just the mavlink portion to a given file
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
#include <string.h>

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
	int i, num, currMess, *ins;
	pcap_t *pcap;
	mavMessArr_t *mav;
	vector_t *vec;

	// Check number of arguments.
	if (argc < 4) {
		printf("usage: extractbymessagenumber inputFile[.pcap]/[.mav] outputFile[.mav] messageNumber[s]\n");
		exit(EXIT_FAILURE);
	}

	// Make sure file exists and can open.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("usage: extractbymessagenumber inputFile[.pcap]/[.mav] outputFile[.mav] messageNumber[s]\n");
		exit(EXIT_FAILURE);
	}

	// Allocate memory.
	if ((vec = vectorInit()) == NULL)
		exit(EXIT_FAILURE);
	
	// Extract which message numbers are to be extacted.
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

	// Deal with the case where a .mav file is being read
	if (strcmp(&argv[1][strlen(argv[1]) - 4] , ".mav") == 0) {
		// Read in file.
		mav = readFileMav(fp);
	}
	else {
		// Read in pcap file
		pcap = readFilePcap(fp);
	}

	fclose(fp);

	// Open file for writing.
	if ((fp = fopen(argv[2], "wb")) == NULL) {
		printf("usage: extractbymessagenumber inputFile[.pcap]/[.mav] outputFile[.mav] messageNumber[s]\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(&argv[1][strlen(argv[1]) - 4] , ".mav") == 0) {
		// Write message to file if number was input
		for (i = 0; i < mav->n; i++) {
			currMess = i + 1;
			if (vectorContains(vec, compareInt, (void *)&(currMess)))
				writeMessageToFileMav(mav->messages[i], fp);
		}

		freeMemMav(mav);
	}
	else {
		// Write message to file if number was input
		for (i = 0; i < pcap->n; i++) {
			currMess = i + 1;
			if (vectorContains(vec, compareInt, (void *)&(currMess)))
				writeMessageToFileMav(pcap->messages[i].body, fp);
		}

		freeMemPcap(pcap);
	}

	fclose(fp);

	vectorApply(vec, free);
	vectorFree(vec);
	exit(EXIT_SUCCESS);
}
