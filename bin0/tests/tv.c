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

int main(){

	//first test with 0
	FILE *fp;

	uint8_t num1, num2,num3;
	num1 = 0;
	num2 = 34;
	num3 = 93;
	fp = fopen("./pass.1", "wb");

	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3,sizeof(uint8_t), 1, fp);
	fclose(fp);

	//empty file test
	fp = fopen("./fail.1","wb");
	fclose(fp);

	//only 0 test
	fp = fopen("./fail.2","wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fclose(fp);

	//no 34 value in middle
	
	fp = fopen("./fail.3","wb");
	num2 = 45;
  fwrite(&num1, sizeof(uint8_t),1 ,fp);                                 
  fwrite(&num2, sizeof(uint8_t), 1, fp);                                
  fwrite(&num3,sizeof(uint8_t), 1, fp);
	fclose(fp);

	num2 = 34;
	
	//more then the 3 values
	fp = fopen("./fail.4","wb");
	uint8_t num4 = 2;
  fwrite(&num1, sizeof(uint8_t),1 ,fp);                                 
  fwrite(&num2, sizeof(uint8_t), 1, fp);                                
  fwrite(&num3,sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
                                                                    
  fclose(fp); 

	
 
	return 0;

}
