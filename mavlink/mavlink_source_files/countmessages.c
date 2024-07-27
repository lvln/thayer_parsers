/* 
 * countmessages.c --- counts the number of messages in a pcap file
 * 
 * Author: Joshua M. Meise
 * Created: 07-26-2024
 * Version: 1.0
 */

#include <mavlink.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	// Variable declarations.
	int num;
	pcap_t * pcap;
	mavMessArr_t *mav;
	FILE *fp;
	
	// Check number of arguments.
	if (argc != 2) {
		printf("usage: countmessages inputFile[.pcap]/[.mav]\n");
		exit(EXIT_FAILURE);
	}

	// Make sure file can open.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("usage: countmessages inputFile[.pcap]/[.mav]\n");
		exit(EXIT_FAILURE);
	}

	// If reading in a mavlink file.
	if (strcmp(&(argv[1][strlen(argv[1]) - 4]), ".mav") == 0) {
		// Read into data structure.
		mav = readMavFile(fp);

		num = mav->n;
		freeMemMav(mav);
	}
	// If reading in a pcap file (default).
	else {
		// Read in the data strucutre.
		pcap = readPcapFile(fp);
		
		num = pcap->n;
		freeMemPcap(pcap);
	}
	
	printf("%d", num);
	fclose(fp);
	exit(EXIT_SUCCESS);
}
