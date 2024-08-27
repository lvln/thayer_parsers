/* 
 * tv.c -- creates test cases
 * 
 * Author: Joshua M. Meise
 * Created: 07-16-2024
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <spp.h>

int main(int argc, char **argv) {
	// Variable declarations.
	int passSeed, failSeed;	

	// Check number of arguments.
	if (argc != 3) {
		printf("usage: tv passSeed failSeed\n");
		exit(EXIT_FAILURE);
	}

	// Convert string to integer.
	passSeed = atoi(argv[1]);
	failSeed = atoi(argv[2]);

	// Generate test cases.
	generateTests(passSeed, failSeed);

	exit(EXIT_SUCCESS);
}
