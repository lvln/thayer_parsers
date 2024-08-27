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

int main(void) {

	FILE *fp;

	uint8_t num1, num2;

	num1 = 0x00;
	num2 = 0x00;

	fp = fopen("./pass.1", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num2 = 0x01;

	fp = fopen("./fail.1", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x01;
	num2 = 0x00;

	fp = fopen("./pass.2", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x0f;

	fp = fopen("./fail.2", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x02;
	num2 = 0x00;

	fp = fopen("./pass.3", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num2 = 0x01;

	fp = fopen("./fail.3", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x03;
	num2 = 0x00;

	fp = fopen("./pass.4", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num2 = 0x04;

	fp = fopen("./fail.4", "wb");
	fwrite(&num1, sizeof(uint8_t), 1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);
	
	return 0;

}
