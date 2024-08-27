/* 
 * bittobyte.h --- interface for bit to byte converter; converts bit fields to byte fields
 * 
 * Author: Joshua M. Meise
 * Created: 08-27-2024
 * Version: 1.0
 * 
 */

#pragma once
#include <stdio.h>

/*
 * Converts a binary file containing bit fields to a binary file containing byte fields.
 * Inputs: input file, output file, array with bit field sizes, number of fields
 * Outputs: 0 is successful, nonzero otherwise
 */
int bitToByte(FILE *ifile, FILE *ofile, int *fieldSizes, int numFields);
