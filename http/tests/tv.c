/* 
 * tv.c --- program to generate test vectors for http grammar
 * 
 * Author: Joshua M. Meise
 * Created: 06-21-2024
 * Version: 1.0
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	FILE *ifile;

	if ((ifile = fopen("pass.1", "w")) == NULL)
		printf("File not successfully opened.\n");
	else {
		fprintf(ifile, "This is a line of text\r\n");
		fclose(ifile);
	}

	exit(EXIT_SUCCESS);
}
	
