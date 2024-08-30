/* 
 * converttobyte.c --- convert bit-based headers to byte-based headers for SPP
 * 
 * Author: Joshua M. Meise
 * Created: 08-30-2024
 * Version: 1.0
 * 
 */

#include <bittobyte.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	// Varibale declarations.
	FILE *ifile, *ofile;

	// Check arguments.
	if (argc != 3) {
		fprintf(stderr, "usage: converttobyte inputFile outputFile\n");
		exit(EXIT_FAILURE);
	}

	// Open files.
	if ((ifile = fopen(argv[1], "rb")) == NULL || (ofile = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file(s).\n");
		exit(EXIT_FAILURE);
	}

	// Convert bit fields to byte fields.
	if (bitToByte(ifile, ofile) != 0)
		fprintf(stderr, "Bit to byte conversion failed.\n");
	
	fclose(ifile);
	fclose(ofile);
	
	exit(EXIT_SUCCESS);
}
