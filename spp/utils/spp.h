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
 * Generate tests for SPP messages.
 * Inputs: pass seed, fail seed
 * Outputs: none
 */
void generateTests(int passSeed, int failSeed);
