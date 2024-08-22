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

	FILE *fp;

	uint8_t num1, num2, num3, num4, num5, num6, num7, num8;

	num1 = 0xb0;
	num2 = 0x04;

	fp = fopen("./pass.1", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.1", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x50;
	num2 = 0xfb;

	fp = fopen("./pass.2", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.2", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x14;
	num2 = 0x05;
	num3 = 0x00;
	num4 = 0x00;

	fp = fopen("./pass.3", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.3", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xec;
	num2 = 0xfa;
	num3 = 0xff;
	num4 = 0xff;

	fp = fopen("./pass.4", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.4", "wb");
	fwrite(&num1, sizeof(uint8_t), 1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x03;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x00;

	fp = fopen("./pass.5", "wb");
	fwrite(&num1, sizeof(uint8_t), 1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1 ,fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x02;

	fp = fopen("./fail.5", "wb");
	fwrite(&num1, sizeof(uint8_t), 1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1 ,fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x01;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x00;

	fp = fopen("./pass.6", "wb");
	fwrite(&num1, sizeof(uint8_t), 1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1 ,fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.6", "wb");
	fwrite(&num1, sizeof(uint8_t), 1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1 ,fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xb0;
	num2 = 0x04;

	fp = fopen("./pass.7", "wb");
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.7", "wb");
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x50;
	num2 = 0xfb;

	fp = fopen("./pass.8", "wb");
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.8", "wb");
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x14;
	num2 = 0x05;
	num3 = 0x00;
	num4 = 0x00;

	fp = fopen("./pass.9", "wb");
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.9", "wb");
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xec;
	num2 = 0xfa;
	num3 = 0xff;
	num4 = 0xff;

	fp = fopen("./pass.10", "wb");
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.10", "wb");
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x03;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x00;

	fp = fopen("./pass.11", "wb");
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x02;

	fp = fopen("./fail.11", "wb");
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x01;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x00;

	fp = fopen("./pass.12", "wb");
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;

	fp = fopen("./fail.12", "wb");
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);
	
	return 0;

}
