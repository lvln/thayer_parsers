/* 
 * tv.c --- 
 * 
 * Author: Stephen Taylor & Brandon Guzman
 * Created: 12-02-2020
 * Version: 1.0
 * 
 * Description: 
 * 
 */

//generate good and bad inputs

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[] ){
	FILE *fp;
	uint8_t i,low,high;

	/* usage: tv [low high] */
	switch(argc) {
	case 1:
		low=0;
		high=255;
		break;
	case 2:
		low = (uint8_t)atoi(argv[1]);		
		high = low;
		break;
	case 3:
		low = (uint8_t)atoi(argv[1]);		
		high = (uint8_t)atoi(argv[2]);
		break;
	default:
		printf("usage: tv [low high]\n");
		printf("   -- create tests on binary values from low to high\n");
		printf("   -- default 0-255, high must be greater than low\n");
		exit(EXIT_SUCCESS);
	}
	if(high<low) {
		printf("[error: invalid range: %d - %d]\n",low,high);
		exit(EXIT_FAILURE);
	}
	printf("[Range: %d - %d]\n",(int)low,(int)high);
	/* generate a test with all the numbers from low to high in order */
	printf("[pass.1: %d bytes]\t-- bytes in order from %d to %d\n",(high-low)+1,low,high);
	if((fp=fopen("./pass.1", "wb"))==NULL) {
		printf("cant open pass.1\n");
		exit(EXIT_FAILURE);
	}
	i=low;
 	for( ; i<high; i++) 
		fwrite(&i, sizeof(uint8_t), 1 , fp);
	fwrite(&i, sizeof(uint8_t), 1 , fp); /* write the last byte */
	fclose(fp);

	//empty file test
	printf("[fail.1: 0 bytes]\t-- no bytes\n");
	if((fp=fopen("./fail.1","wb"))==NULL) {
		printf("cant open fail.1\n");
		exit(EXIT_FAILURE);
	}
	fclose(fp);

	if(high>low) {
		// last value duplicated
		printf("[fail.2: %d bytes]\t-- last value duplicated\n",(high-low)+1);
		if((fp=fopen("./fail.2","wb"))==NULL) {
			printf("cant open fail.2\n");
			exit(EXIT_FAILURE);
		}
		for(i=low; i<high; i++)
			fwrite(&i, sizeof(uint8_t), 1 , fp);
		i -= 1;
		fwrite(&i, sizeof(uint8_t), 1 , fp);
		fclose(fp);

		//out of order last pair
		printf("[fail.3: %d bytes]\t-- last two values reversed\n",(high-low)+1);
		if((fp=fopen("./fail.3","wb"))==NULL) {
			printf("cant open fail.3\n");
			exit(EXIT_FAILURE);
		}
		for(i=low; i<high-1; i++)
			fwrite(&i, sizeof(uint8_t), 1 , fp);
		i=i+1;
		fwrite(&i, sizeof(uint8_t), 1 , fp);
		i=i-1;
		fwrite(&i, sizeof(uint8_t), 1 , fp);
		fclose(fp);

		//short file
		printf("[fail.4: %d bytes]\t-- short file\n",(high-low));
		if((fp=fopen("./fail.4","wb"))==NULL) {
			printf("cant open fail.4\n");
			exit(EXIT_FAILURE);
		}
		for(i=low; i<high; i++)
			fwrite(&i, sizeof(uint8_t), 1 , fp);
		fclose(fp);
	}
	else {
		// single value and its wrong
		printf("[fail.2: 1 byte]\t-- value wrong\n");
		if((fp=fopen("./fail.2","wb"))==NULL) {
			printf("cant open fail.2\n");
			exit(EXIT_FAILURE);
		}
		i=low-1;
		fwrite(&i, sizeof(uint8_t), 1 , fp);
		fclose(fp);
	}
	exit(EXIT_SUCCESS);
}
