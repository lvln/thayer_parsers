/* 
 * echo.c --- echoes the binary characters from an input file to the screen in hexadecimal format
 * 
 * Author: Joshua M. Meise
 * Created: 06-26-2024
 * Version: 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char **argv) {
	FILE *ifile;
	uint8_t buf;
	int bytesRead;
	
	if (argc != 2) {                                                                                                                                                                                         
    printf("usage: echo inputFile\n");
    exit(EXIT_FAILURE);
	}

	if ((ifile = fopen(argv[1], "rb")) == NULL) {
    printf("File does not exist\n");
    exit(EXIT_FAILURE);
	}

	while ((bytesRead = fread(&buf, 1, sizeof(buf), ifile)) > 0)
		printf("%02x ", buf);
	
	exit(EXIT_SUCCESS);
}
