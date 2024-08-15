/* 
 * countids.c --- counts the number of unique message IDs in a file
 * 
 * Author: Joshua M. Meise
 * Created: 08-14-2024
 * Version: 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	// Variable declarations.
	FILE *fp;
	mavlink_t *m;
	pcap_t *p;
	tlog_t *t;
	bool pcap, mav, tlog;
	
	// Check number of arguments.
	if (argc != 2) {
		fprintf(stderr, "usage: countids inputFile[.mav|.tlog|.pcap]\n");
		exit(EXIT_FAILURE);
	}

	// Open file.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Failed to open file %s.\n", argv[1]);
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

	// Read each file and count the codes.
	if (pcap) {
		p = readFilePcap(fp);
		countIDSPcap(p);
		freePcap(p);
	}
	else if (tlog) {
		t = readFileTlog(fp);
		countIDSTlog(t);
		freeTlog(t);
	}
	else if (mav) {
		m = readFileMav(fp);
		countIDSMav(m);
		freeMav(m);
	}

	fclose(fp);
	
	exit(EXIT_SUCCESS);
}
