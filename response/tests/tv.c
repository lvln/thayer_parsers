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

int main(){


	FILE *fp;

	uint8_t ID = 1;
	uint8_t tp1 = 64;
	uint8_t tp2 = 80;
	uint8_t tp3 = 96;
	uint8_t tp4 = 112;
	uint8_t tp5 = 128;
	uint8_t tp6 = 129;

	fp = fopen("./pass.1", "wb");
	if (fp == NULL) return 1;
	fwrite(&tp1, sizeof(uint8_t), 1, fp);
	fwrite(&ID, sizeof(uint8_t),1,fp);
	fclose(fp);

	ID = 40;
	fp = fopen("./pass.2", "wb");                                                                                                                     
  if (fp == NULL) return 1;                                                                                                                           
  fwrite(&tp2, sizeof(uint8_t), 1, fp);                                                                                                               
  fwrite(&ID, sizeof(uint8_t),1,fp);                                                                                                                  
  fclose(fp);

	ID = 63;                                                                                                                                            
  fp = fopen("./pass.3", "wb");                                                                                                                       
  if (fp == NULL) return 1;                                                                                                                           
  fwrite(&tp3, sizeof(uint8_t), 1, fp);                                                                                                               
  fwrite(&ID, sizeof(uint8_t),1,fp);                                                                                                                  
  fclose(fp);

	ID = 189;                                                                                                                                           
  fp = fopen("./pass.4", "wb");                                                                                                                       
  if (fp == NULL) return 1;                                                                                                                           
  fwrite(&tp4, sizeof(uint8_t), 1, fp);                                                                                                               
  fwrite(&ID, sizeof(uint8_t),1,fp);                                                                                                                  
  fclose(fp);

	ID = 200;                                                                                                                                           
  fp = fopen("./pass.5", "wb");                                                                                                                       
  if (fp == NULL) return 1;                                                                                                                           
  fwrite(&tp5, sizeof(uint8_t), 1, fp);                                                                                                               
  fwrite(&ID, sizeof(uint8_t),1,fp);                                                                                                                  
  fclose(fp);

	ID = 255;                                                                                                                                           
  fp = fopen("./pass.6", "wb");                                                                                                                       
  if (fp == NULL) return 1;                                                                                                                           
  fwrite(&tp6, sizeof(uint8_t), 1, fp);                                                                                                               
  fwrite(&ID, sizeof(uint8_t),1,fp);                                                                                                                  
  fclose(fp);
	
	//non numbers 
  fp = fopen("./fail.1", "wb");                                                                                                                       
  if (fp == NULL) return 1;
	char a = 'a';
  fwrite(&a, sizeof(char), 1, fp);                                                                                                               
  fclose(fp);

	//wrong data type 
  fp = fopen("./fail.2", "wb");                                                                                                                       
  if (fp == NULL) return 1;
	int32_t num = 32760;
  fwrite(&num, sizeof(int32_t), 1, fp);
	fwrite(&ID, sizeof(uint8_t),1,fp);  
  fclose(fp);

	//wrong type
	fp = fopen("./fail.3", "wb");                                                                                                                       
  if (fp == NULL) return 1;                                                                                                                           
  uint8_t tp7 = 73;                                                                                                                                
  fwrite(&tp7, sizeof(int32_t), 1, fp);
	fwrite(&ID, sizeof(uint8_t),1,fp);  
  fclose(fp);
	
	return 0;
}
