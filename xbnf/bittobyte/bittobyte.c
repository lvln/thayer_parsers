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

/*
 * Converts a binary file containing bit fields to a binary file containing byte fields.
 * Inputs: input file, output file, array with bit field sizes, number of fields
 * Outputs: 0 is successful, nonzero otherwise
 */
int bitToByte(FILE *ifile, FILE *ofile, int *fieldSizes, int numFields) {
	// Variable declarations.
	int numBits, i, j, bitCnt, bit;
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
		
		for (j = 0; j < numBits; j++) {
			if (bitCnt == 0)
				if (fread(&ibyte, sizeof(uint8_t), i, ifile) != 1)
					fprintf(stderr, "Byte not read.\n");

			// Extract most significant bit.
			bit = (ibyte >> 7) & 1;

			// Shift left by 1.
			ibyte <<= 1;
			
			// Shift the bit into the output byte.
			obyte |= bit << (numBits - 1 - j);
			
			if (bitCnt == 7) bitCnt = 0;
			else bitCnt++;
		}
		
	}
	
	// Close files.
	fclose(ifile);
	fclose(ofile);
	
	return 0;
}
