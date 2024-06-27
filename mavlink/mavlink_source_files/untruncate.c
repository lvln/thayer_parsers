/* 
 * analyze.c -- runs analysis on the data captured in a .cap file
 * 
 * Author: Joshua M. Meise
 * Created: 06-26-2024
 * Version: 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <mavlink.h>

int main(int argc, char **argv) {
	// Variable declarations
	FILE *ifile, *ofile;
	int i, numMess;
	pcap_t *pcapFile;

	// Check arguments
	if (argc != 3) {
    printf("usage: untruncate inputFile outputFile\n");
    exit(EXIT_FAILURE);
	}

	// Check that provided file exists by opening for binary reading
	if ((ifile = fopen(argv[1], "rb")) == NULL) {
		printf("usage: untruncate inputFile outputFile\n");
		exit(EXIT_FAILURE);
	}
	
	i = 0;

	if ((ofile = fopen(argv[2], "wb")) == NULL) {
		printf("usage: untruncate inputFile outputFile\n");
		exit(EXIT_FAILURE);
	}

	if ((pcapFile = readPcapFile(ifile)) == NULL)
		printf("File reading failed.\n");

	writeHeader(pcapFile, ofile);
	
	numMess = pcapFile->n;
	
	for (i = 0; i < numMess; i++) {
		printMessage(pcapFile->messages[i]);
		untruncate(&(pcapFile->messages[i]));
		printMessage(pcapFile->messages[i]);
		writeToFile(pcapFile->messages[i], ofile);
	}

	freeMem(pcapFile);

	fclose(ofile);
	fclose(ifile);
	exit(EXIT_SUCCESS);
}
