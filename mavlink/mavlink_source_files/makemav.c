/* 
 * makemav.c --- creates a file with only MAVlink messages given an input file of a PCAP or TLOG file
 * 
 * Author: Joshua M. Meise
 * Created: 07-23-2024
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	// Variable declarations.
	FILE *fp;
	pcap_t *p;
	tlog_t *t;
	mavlink_t *m;
	bool pcap, tlog;
	
	// Check number of argumnets.
	if (argc != 3) {
		fprintf(stderr, "usage: makemav inputFile [.tlog|.pcap] outputFile[.mav]\n");
		exit(EXIT_FAILURE);
	}

	// Open file.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	
	pcap = tlog = false;
	
	// Check end of file name to see extension; default to TLOG file.
	if (strlen(argv[1]) > 5) {
		if (strcmp(&argv[1][strlen(argv[1]) - 5], ".pcap") == 0)
			pcap = true;
		else if (strcmp(&argv[1][strlen(argv[1]) - 5], ".tlog") == 0)
			tlog = true;
	}
	if (!tlog && !pcap) tlog = true;
	
	// Convert to MAVLink.
	if (pcap) {
		if ((p = readFilePcap(fp)) == NULL) {
			fprintf(stderr, "Failed to read file.\n");
			exit(EXIT_FAILURE);
		}
		
		if ((m = pcapToMav(p)) == NULL) {
			fprintf(stderr, "Failed to convert to MAVLink.\n");
			exit(EXIT_FAILURE);
		}

		freePcap(p);
	}
	else if (tlog) {
		if ((t = readFileTlog(fp)) == NULL) {
			fprintf(stderr, "Failed to read file.\n");
			exit(EXIT_FAILURE);
		}

		if ((m = tlogToMav(t)) == NULL) {
			fprintf(stderr, "Failed to convert to MAVLink.\n");
			exit(EXIT_FAILURE);
		}

		freeTlog(t);
	}

	fclose(fp);
	
	// Open file.
	if ((fp = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		exit(EXIT_FAILURE);
	}
	
	// Write messages to file.
	writeToFileMav(m, fp);
	
	fclose(fp);
	freeMav(m);
	exit(EXIT_SUCCESS);
}
