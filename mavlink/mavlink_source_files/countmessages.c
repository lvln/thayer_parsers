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

int main(int argc, char **argv) {
	// Variable declarations.
	int num;
	pcap_t * pcap;
	FILE *fp;
	
	// Check number of arguments.
	if (argc != 2) {
		printf("usage: countmessages inputFile[.pcap]\n");
		exit(EXIT_FAILURE);
	}

	// Make sure file can open.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("usage: countmessages inputFile[.pcap]\n");
		exit(EXIT_FAILURE);
	}

	// Read in the data strucutre.
	pcap = readPcapFile(fp);

	fclose(fp);

	num = pcap->n;
	freeMemPcap(pcap);
	printf("%d", num);

	exit(EXIT_SUCCESS);
}
