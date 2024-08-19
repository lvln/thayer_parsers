/* 
 * tvsingle.c --- generates a single passing test for a given message type
 * 
 * Author: Joshua M. Meise
 * Created: 08-15-2024
 * Version: 1.0
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>

int main(int argc, char **argv) {
	// Variable declarations.
	int id, passSeed;	

	// Check number of arguments.
	if (argc != 3) {
		fprintf(stderr, "usage: tvsingle msgID passSeed\n");
		exit(EXIT_FAILURE);
	}

	// Convert string to integer.
	id = atoi(argv[1]);
	passSeed = atoi(argv[2]);

	// Generate test cases for the given message id.
	generateSingleTest(id, passSeed);

	exit(EXIT_SUCCESS);
}
