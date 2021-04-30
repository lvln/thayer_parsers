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
#include <stdbool.h>

int main(int argc, char *argv[] ){
	FILE *fp;
	char fn[256];
	uint8_t i,low,high;
	int j;
	bool done;

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
		printf("   -- create a set of one byte tests on binary values from low to high\n");
		printf("   -- default 0-255, high must be greater than low\n");
		exit(EXIT_SUCCESS);
	}
	if(high<low) {
		printf("[error: invalid range: %d - %d]\n",low,high);
		exit(EXIT_FAILURE);
	}
	printf("[Range: %d - %d]\n",(int)low,(int)high);
	for(done=false,j=1,i=low; !done; i++,j++) {
		/* generate a test with all the numbers from low to high in order */
		printf("[pass.%d] : %d\n",j,i);
		sprintf(fn,"pass.%d",j);
		if((fp=fopen(fn, "wb"))==NULL) {
			printf("cant open pass.1\n");
			exit(EXIT_FAILURE);
		}
		fwrite(&i, sizeof(uint8_t), 1 , fp);
		fclose(fp);
		if(i==high)
			done=true;
	}
	//empty file test
	printf("[fail.1: 0 bytes]\t-- no bytes\n");
	if((fp=fopen("./fail.1","wb"))==NULL) {
		printf("cant open fail.1\n");
		exit(EXIT_FAILURE);
	}
	fclose(fp);

	//two bytes should fail
	printf("[fail.2: 2 bytes]\t-- 2 bytes\n");
	if((fp=fopen("./fail.2","wb"))==NULL) {
		printf("cant open fail.1\n");
		exit(EXIT_FAILURE);
	}
	i=0;
	fwrite(&i, sizeof(uint8_t), 1 , fp);	
	i=255;
	fwrite(&i, sizeof(uint8_t), 1 , fp);	
	fclose(fp);

	exit(EXIT_SUCCESS);
}
