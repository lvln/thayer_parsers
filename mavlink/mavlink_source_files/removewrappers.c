/* 
 * removewrappers.c --- removes all PCAP wrappers from a datafile and outouts a .mav file containing only MAVLink messages.
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
	char fname[50], mavname[54];
	int i;
	
	// Check number of argumnets.
	if (argc != 2) {
		printf("usage: removewrappers inputFile\n");
		exit(EXIT_FAILURE);
	}

	// Make sure it is a pcap file.
	if (strcmp(&argv[1][strlen(argv[1]) - 5], ".pcap") != 0) {
		printf("usage: removewrappers inputFile\n");
		exit(EXIT_FAILURE);
	}

	// Open file.
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("usage: removewrappers inputFile\n");
		exit(EXIT_FAILURE);
	}

	// Read the file contents into data structure.
	pcapFile = readPcapFile(fp);

	fclose(fp);

	// Create a new file name with .mav extension.
	strncpy(fname, argv[1], strlen(argv[1] - 5));
	sprintf(mavname, "%s.mav", fname);

	// Open file.
	if ((fp = fopen(mavname, "wb")) == NULL) {
		printf("File not opened for writing.\n");
		exit(EXIT_FAILURE);
	}
	
	// Write only the MAVLink portion of each message to the file.
	for (i = 0; i < pcapFile->n; i++)
		writeMavMessageToFile(pcapFile->messages[i].body, fp);

	fclose(fp);

	freeMemPcap(pcapFile);
	
	exit(EXIT_SUCCESS);
}
