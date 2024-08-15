/* 
 * extractbymessageid.c --- extracts all messages of a given message id from a given file and outputs to a MAVLink file
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

int main(int argc, char **argv) {
	// Variable declarations.
	FILE *fp;
	int i, num, *ins;
	pcap_t *p;
	mavlink_t *m, *out;
	tlog_t *t;
	vector_t *vec;
	bool pcap, mav, tlog;
	
	// Check number of arguments.
	if (argc < 4) {
		fprintf(stderr, "usage: extractbymessageid inputFile[.mav|.pcap|.tlog] outputFile[.mav] messageID[s]\n");
		exit(EXIT_FAILURE);
	}

	// Make sure file exists and can open.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Could not open file.\n");
		exit(EXIT_FAILURE);
	}

	pcap = mav = tlog = false;
	
	// Check end of file name to see extension; default to MAVLink file.
	if (strlen(argv[1]) > 5) {
		if (strcmp(&argv[1][strlen(argv[1]) - 5], ".pcap") == 0)
			pcap = true;
		else if (strcmp(&argv[1][strlen(argv[1]) - 5], ".tlog") == 0)
			tlog = true;
	}

	if (!pcap && !tlog)
		mav = true;

	// Allocate memory.
	if ((vec = vectorInit()) == NULL) {
		fprintf(stderr, "Vector initialisation failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Extract which message ids are to be extacted.
	for (i = 0; i < argc - 3; i++) {
		if ((num = atoi(argv[i + 3])) < 0) {
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
		if (vectorInsertBack(vec, (void *)ins) != 0)
			fprintf(stderr, "Vector insertion failed.\n");
	}
	
	// Depending on type of file extract the messages and place into MAVLink structure.
	if (pcap) {
		if ((p = readFilePcap(fp)) == NULL) {
			fprintf(stderr, "Error reading file.\n");
			exit(EXIT_FAILURE);
		}
		
		if ((out = extractByIdPcapToMav(p, vec)) == NULL) {
			fprintf(stderr, "Error extracting messages.\n");
			exit(EXIT_FAILURE);
		}

		freePcap(p);
	}
	else if (tlog) {
		if ((t = readFileTlog(fp)) == NULL) {
			fprintf(stderr, "Error reading file.\n");
			exit(EXIT_FAILURE);
		}

		if ((out = extractByIdTlogToMav(t, vec)) == NULL) {
			fprintf(stderr, "Error extracting messages.\n");
			exit(EXIT_FAILURE);
		}

		freeTlog(t);
	}
	else if (mav) {
		if ((m = readFileMav(fp)) == NULL) {
			fprintf(stderr, "Error reading file.\n");
			exit(EXIT_FAILURE);
		}

		if ((out = extractByIdMavToMav(m, vec)) == NULL) {
			fprintf(stderr, "Error extracting messages.\n");
			exit(EXIT_FAILURE);
		}

		freeMav(m);
	}

	fclose(fp);

	// Open file for writing.
	if ((fp = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}

	// Write messages to new file.
	writeToFileMav(out, fp);

	fclose(fp);

	freeMav(out);
	vectorApply(vec, free);
	vectorFree(vec);
	exit(EXIT_SUCCESS);
}
