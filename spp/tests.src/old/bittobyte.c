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
 * Inputs: input file, output file
 * Outputs: 0 is successful, nonzero otherwise
 */
int bitToByte(FILE *ifile, FILE *ofile) {
	// Variable declarations.
	uint8_t numBits, numBytes, i, j, bitCnt, bit, fieldSizes[7], ibyte, obyte;
	
	// Check arguments.
	if (!ifile || !ofile) {
		fprintf(stderr, "Invalid argument(s).\n");
		return 1;
	}

	// Array of number of bits per field for space packet protocol.
	fieldSizes[0] = 3;
	fieldSizes[1] = 1;
	fieldSizes[2] = 1;
	fieldSizes[3] = 11;
	fieldSizes[4] = 2;
	fieldSizes[5] = 14;
	fieldSizes[6] = 16;

	bitCnt = 0;
	
	for (i = 0; i < 7; i++) {
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
