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
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

uint8_t random_uint(int min, int max){
	srand(time(0));
	int n = rand() % (max + 1 - min)+min;
	uint8_t k = (uint8_t) n;
	return k;

}

int main(){


	FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6, *fp7, *fp8, *fp9, *fp10, *fp11, *fp12;

	uint8_t byte1_edge1, byte1_edge2, byte2_edge1,  byte2_edge2, byte3_edge1, byte3_edge2, byte4_edge1, byte4_edge2, continued1, continued2;
	

	byte1_edge1 = 0;
	byte1_edge2 = 127;

	//edge cases for 1 byte
	fp1 = fopen("./pass.1", "wb");
	fp2 = fopen("./pass.2", "wb");

	fwrite(&byte1_edge1, sizeof(uint8_t),1 ,fp1);
	fclose(fp1);
	fwrite(&byte1_edge2, sizeof(uint8_t), 1, fp2);
	fclose(fp2);

	//edge cases for 2 bytes 
	//Left edge
	byte2_edge1= 194;
	continued1 = 128;

	fp3 = fopen("./pass.3", "wb");
	fwrite(&byte2_edge1, sizeof(uint8_t),1 ,fp3);
	fwrite(&continued1, sizeof(uint8_t),1 ,fp3);

	fclose(fp3);

	//edge cases for 2 bytes 
	//Right edge
	byte2_edge2 = 223;
	continued2 = 191;

	fp4 = fopen("./pass.4", "wb");
	fwrite(&byte2_edge2, sizeof(uint8_t),1 ,fp4);
	fwrite(&continued2, sizeof(uint8_t),1 ,fp4);

	fclose(fp4);


	//3 bytes

	byte3_edge1 = 224;
	
	fp5 = fopen("./pass.5", "wb");
	fwrite(&byte3_edge1, sizeof(uint8_t),1 ,fp5);
	fwrite(&continued1, sizeof(uint8_t),1 ,fp5);
	fwrite(&continued1, sizeof(uint8_t),1 ,fp5);

	fclose(fp5);

	byte3_edge2 = 239;

	fp6 = fopen("./pass.6", "wb");
	fwrite(&byte3_edge2, sizeof(uint8_t),1 ,fp5);
	fwrite(&continued2, sizeof(uint8_t),1 ,fp5);
	fwrite(&continued2, sizeof(uint8_t),1 ,fp5);

	fclose(fp6);


	byte4_edge1 = 240;
	byte4_edge2 = 255;

	fp7 = fopen("./pass.7", "wb");
	fwrite(&byte4_edge1, sizeof(uint8_t),1 ,fp7);
	fwrite(&continued1, sizeof(uint8_t),1 ,fp7);
	fwrite(&continued1, sizeof(uint8_t),1 ,fp7);
	fwrite(&continued1, sizeof(uint8_t),1 ,fp7);

	fclose(fp7);

	fp8 = fopen("./pass.8", "wb");
	fwrite(&byte4_edge2, sizeof(uint8_t),1 ,fp8);
	fwrite(&continued2, sizeof(uint8_t),1 ,fp8);
	fwrite(&continued2, sizeof(uint8_t),1 ,fp8);
	fwrite(&continued2, sizeof(uint8_t),1 ,fp8);

	fclose(fp8);

	//Random Testing: Generate random unicode 

	//Random 1 byte

	uint8_t random1b = random_uint(0,127);

	fp9 = fopen("./pass.9", "wb");
	fwrite(&random1b, sizeof(uint8_t),1 ,fp9);

	fclose(fp9);

	//Random 2 byte

	uint8_t random2b = random_uint(194,223);
	uint8_t random_continued = random_uint(128,191);

	fp10 = fopen("./pass.10", "wb");
	fwrite(&random2b, sizeof(uint8_t),1 ,fp10);
	fwrite(&random_continued, sizeof(uint8_t),1 ,fp10);

	fclose(fp10);

	//Random 3 bytes

	uint8_t random3b = random_uint(224,239);
	

	fp11 = fopen("./pass.11", "wb");
	fwrite(&random3b, sizeof(uint8_t),1 ,fp11);
	for (int i = 0; i < 2; i ++){
		random_continued = random_uint(128,191);
		fwrite(&random_continued, sizeof(uint8_t),1 ,fp11);
	}
	
	fclose(fp11);

	//Random 4 bytes

	uint8_t random4b = random_uint(240,255);
	

	fp12 = fopen("./pass.12", "wb");
	fwrite(&random4b, sizeof(uint8_t),1 ,fp12);
	for (int i = 0; i < 3; i ++){
		random_continued = random_uint(128,191);
		fwrite(&random_continued, sizeof(uint8_t),1 ,fp12);
	}

	fclose(fp12);


	//Failure cases

	FILE *fp13, *fp14, *fp15, *fp16, *fp17,*fp18 ,*fp19 , *fp20, *fp21,*fp22, *fp23, *fp24;
	uint8_t fail1, fail2, fail_first_byte, fail_second_byte, fail_third_byte, fail_fourth_byte;
	
	//empty file test

	fp13 = fopen("./fail.1","wb");
	fclose(fp13);

 	//1 byte: out of range
	fail1 = -1;
	fp14 = fopen("./fail.2","wb");

	fwrite(&fail1, sizeof(uint8_t),1 ,fp14);
	fclose(fp14);

	fail2 = 128;
	fp15 = fopen("./fail.3","wb");

	fwrite(&fail2, sizeof(uint8_t),1 ,fp15);
	fclose(fp15);

 	//2 byte: first out of range 
	
	fail_first_byte = 193;
	fail_second_byte = 130;
	fp16 = fopen("./fail.4","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp16);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp16);
	fclose(fp16);

	//2 byte: second out of range 
	fail_first_byte = 195;
	fail_second_byte = 192;
	fp17 = fopen("./fail.5","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp17);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp17);
	fclose(fp17);

	//3 byte: first out of range  

	fail_first_byte = 245; 
	fail_second_byte = 130;
	fail_third_byte = 130;
	fp18 = fopen("./fail.6","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp18);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp18);
	fwrite(&fail_third_byte, sizeof(uint8_t),1 ,fp18);
	fclose(fp18);

	//3 byte: second out of range 

	fail_first_byte = 230;
	fail_second_byte = 126;
	fp19 = fopen("./fail.7","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp19);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp19);
	fwrite(&fail_third_byte, sizeof(uint8_t),1 ,fp19);
	fclose(fp19);

	//3 byte: third out of range 
	fail_second_byte = 130;
	fail_third_byte = 126;
	fp20 = fopen("./fail.8","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp20);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp20);
	fwrite(&fail_third_byte, sizeof(uint8_t),1 ,fp20);
	fclose(fp20);

	//4 byte: first out of range 

	fail_first_byte = 1; 
	fail_second_byte = 130;
	fail_third_byte = 130;
	fail_fourth_byte = 130; 
	fp21 = fopen("./fail.9","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp21);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp21);
	fwrite(&fail_third_byte, sizeof(uint8_t),1 ,fp21);
	fwrite(&fail_fourth_byte, sizeof(uint8_t),1 ,fp21);
	fclose(fp21);

	//4 byte: second out of range 

	fail_first_byte = 250;
	fail_second_byte = 127;
	fail_third_byte = 130;
	fail_fourth_byte = 130;
	fp22 = fopen("./fail.10","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp22);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp22);
	fwrite(&fail_third_byte, sizeof(uint8_t),1 ,fp22);
	fwrite(&fail_fourth_byte, sizeof(uint8_t),1 ,fp22);
	fclose(fp22);

	//4 byte: third out of range 

	fail_first_byte = 250;
	fail_second_byte = 130;
	fail_third_byte = 127;
	fail_fourth_byte = 130;
	fp23 = fopen("./fail.11","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp23);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp23);
	fwrite(&fail_third_byte, sizeof(uint8_t),1 ,fp23);
	fwrite(&fail_fourth_byte, sizeof(uint8_t),1 ,fp23);
	fclose(fp23);

	 //4 byte: fourth out of range 

	fail_first_byte = 250;
	fail_second_byte = 130;
	fail_third_byte = 130;
	fail_fourth_byte = 127;
	fp24 = fopen("./fail.12","wb");

	fwrite(&fail_first_byte , sizeof(uint8_t),1 ,fp24);
	fwrite(&fail_second_byte, sizeof(uint8_t),1 ,fp24);
	fwrite(&fail_third_byte, sizeof(uint8_t),1 ,fp24);
	fwrite(&fail_fourth_byte, sizeof(uint8_t),1 ,fp24);
	fclose(fp24);

	return 0;

}
