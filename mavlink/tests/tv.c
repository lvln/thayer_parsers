/* 
 * tv.c -- creates test cases for a given message based on its message id
 *         creates passing and failing tests for every byte value in every payload field, as well as header field
 * 
 * Author: Joshua M. Meise
 * Created: 07-16-2024
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>

int main(int argc, char **argv) {
	// Variable declarations.
	int id, passSeed, failSeed;	

	// Check number of arguments.
	if (argc != 4) {
		printf("usage: tv msgID passSeed failSeed\n");
		exit(EXIT_FAILURE);
	}

	// Convert string to integer.
	id = atoi(argv[1]);
	passSeed = atoi(argv[2]);
	failSeed = atoi(argv[3]);

	// Generate test cases for the given message id.
	generateTests(id, passSeed, failSeed);

	exit(EXIT_SUCCESS);
}
