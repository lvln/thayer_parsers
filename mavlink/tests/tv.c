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
#include <stdbool.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
	// Variable declarations.
	int id;
	

	if (argc != 2) {
		printf("usage: tv msgID\n");
		exit(EXIT_FAILURE);
	}

	id = atoi(argv[1]);
	
	generateTests(id);

	exit(EXIT_SUCCESS);
}
