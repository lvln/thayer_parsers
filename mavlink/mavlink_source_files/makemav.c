/* 
 * makemav.c --- removes all PCAP wrappers from a datafile and outputs a .mav file containing only MAVLink messages.
 * 
 * Author: Joshua M. Meise
 * Created: 07-23-2024
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>
#include <string.h>

int main(int argc, char **argv) {
	// Variable declarations.
	FILE *fp;
	pcap_t *pcapFile;
	int i;
	
	// Check number of argumnets.
	if (argc != 3) {
		printf("usage: makemav inputFile.pcap outputFile.mav\n");
		exit(EXIT_FAILURE);
	}

	// Make sure it is a pcap file.
	if (strcmp(&argv[1][strlen(argv[1]) - 5], ".pcap") != 0) {
		printf("usage: makemav inputFile.pcap outputFile.mav\n");
		exit(EXIT_FAILURE);
	}

	// Open file.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("usage: makemav inputFile.pcap outputFile.mav\n");
		exit(EXIT_FAILURE);
	}

	// Read the file contents into data structure.
	pcapFile = readFilePcap(fp);

	fclose(fp);

	// Open file.
	if ((fp = fopen(argv[2], "wb")) == NULL) {
		printf("usage: makemav inputFile.pcap outputFile.mav\n");
		exit(EXIT_FAILURE);
	}

	// Write only the MAVLink portion of each message to the file.
	for (i = 0; i < pcapFile->n; i++)
		writeMessageToFileMav(pcapFile->messages[i].body, fp);

	fclose(fp);

	freeMemPcap(pcapFile);
	
	exit(EXIT_SUCCESS);
}
