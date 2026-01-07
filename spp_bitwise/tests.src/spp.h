/*
 * spp.h --- Space Packet Protocol (SPP) data processing and fuzzing library
 * 
 * Author: Joshua M. Meise
 * Created: 08-26-2024
 * Version: 1.0
 * 
 */

#pragma once
#include <stdio.h>

/*
 * Public repreentation of SPP data structure.
 */
typedef void spp_t;

/*
 * Write SPP test to file.
 * Inputs: SPP data structure, file to which to write
 * Outputs: none
 */
void writeSppToFile(spp_t *spp, FILE *fp);

/*
 * Print SPP messgaes.
 * Inputs: SPP data structure
 * Outputs: none
 */
void printSpp(spp_t *spp);


/*
 * Reads SPP message from a binary file.
 * Inputs: File from which to read.
 * Outputs: SPP data structure, NULL if unsuccessful
 */
spp_t *readSPPFile(FILE *ifile);

/*
 * Generate tests for SPP messages.
 * Inputs: pass seed, fail seed
 * Outputs: none
 */
void generateTests(int passSeed, int failSeed);

/*
 * Generate failure test for spp message for a given set of passing messages
 * Inputs: fail seed, SPP data structure (may contain 0 or more messages)
 * Outputs: none
 */
void generateFailTest(int failSeed, spp_t *spp);
