/* 
 * tv.c --- 
 * 
 * Author: Joshua Renfro
 * Created: 01-10-2023
 * Version: 1.0
 * 
 * Description: 
 * 
 */

//generate good and bad inputs

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "data.h"

int main(){
	FILE *fp;
	char fname[8096];
	uint8_t input[10000];
	for(int ii = 0; ii < num_messages; ii++)
	{
		sprintf(fname, "./pass.%d", ii+1);
		fp = fopen(fname, "wb");
		memcpy(input, messagesPass[ii].bytes, messagesPass[ii].size);
		fwrite(input, sizeof(messagesPass[ii].bytes[0]),messagesPass[ii].size ,fp);
		fclose(fp);
	}

	/*//empty file test
	fp = fopen("./fail.1","wb");
	fclose(fp);*/
	
	for(int ii = 0; ii < num_messages; ii++)
	{
		sprintf(fname, "./fail.%d", ii+1);
		fp = fopen(fname, "wb");
		memcpy(input, messagesFail[ii].bytes, messagesFail[ii].size);
		fwrite(input, sizeof(messagesFail[ii].bytes[0]),messagesFail[ii].size ,fp);
		fclose(fp);
	}
 
	return 0;

}
