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
	int id;	

	// Check number of arguments.
	if (argc != 2) {
		printf("usage: tv msgID\n");
		exit(EXIT_FAILURE);
	}

	// Convert string to integer.
	id = atoi(argv[1]);

	// Generate test cases for the given message id.
	generateTests(id);

	exit(EXIT_SUCCESS);
}
