/* 
 * bittobyte.c --- converts bit fields to byte fields
 * 
 * Author: Joshua M. Meise
 * Created: 08-26-2024
 * Version: 1.0
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Converts a binary file containing bit fields to a binary file containing byte fields.
 * Inputs: input file, output file, array with bit field sizes, number of fields
 * Outputs: 0 is successful, nonzero otherwise
 */
int bitToByte(FILE *ifile, FILE *ofile, int *fieldSizes, int numFields) {
	// Variable declarations.
	int numBits, numBytes, i, j, bitCnt, bit;
	uint8_t ibyte, obyte;
	
	// Check arguments.
	if (!ifile || !ofile || !fieldSizes || numFields < 1) {
		fprintf(stderr, "Invalid argument(s).\n");
		return 1;
	}

	bitCnt = 0;
	
	for (i = 0; i < numFields; i++) {
		numBits = fieldSizes[i];
		obyte = 0;
		numBytes = (numBits - 1)/8;
		
		for (j = 0; j < numBits; j++) {
			// Read another byte each time 8 bits have been extracted.
			if (bitCnt == 0)
				if (fread(&ibyte, sizeof(uint8_t), 1, ifile) != 1)
					fprintf(stderr, "Byte not read.\n");
			
			// Extract most significant bit.
			bit = (ibyte >> 7) & 1;

			// Shift left by 1.
			ibyte <<= 1;
			
			// Shift the bit into the output byte.
			obyte |= bit << (numBits - 1 - j - 8*numBytes);

			// Once 8 bts have been extracted, write byte to file.
			if (bitCnt == 7) {
				bitCnt = 0;
				
				if (fwrite(&obyte, sizeof(uint8_t), 1, ofile) != 1)
					fprintf(stderr, "Problem writing byte %02x to file.\n", obyte);
				
				obyte = 0;
				numBytes--;
			}
			else bitCnt++;
		}

		// Write to file is the relevant number of bits have been extracted and have not yet reached the en fo the input byte.
		if (bitCnt != 0)
			if (fwrite(&obyte, sizeof(uint8_t), 1, ofile) != 1)
				fprintf(stderr, "Problem writing byte %02x to file.\n", obyte);
	}

	// Write the remainder of the bytes to file.
	while (fread(&ibyte, sizeof(uint8_t), 1, ifile) == 1)
		if (fwrite(&ibyte, sizeof(uint8_t), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file.\n", ibyte);
	
	return 0;
}
