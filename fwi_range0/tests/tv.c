/* 
 * tv.c --- 
 * 
 * Author: Josh Meise
 * Created: 30-08-2024
 * Version: 1.0
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdint.h>

int main(void) {

	FILE *fp;

	uint8_t num1, num2, num3, num4, num5, num6, num7, num8;

	num1 = 0x00;
	num2 = 0x00;

	fp = fopen("./pass.1", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x01;
	num2 = 0x00;

	fp = fopen("./pass.2", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x02;
	num2 = 0x00;

	fp = fopen("./pass.3", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);
	
	num1 = 0x03;
	num2 = 0x00;

	fp = fopen("./pass.4", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x04;
	num2 = 0x00;

	fp = fopen("./fail.1", "wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xff;
	num2 = 0xff;
	num3 = 0xff;
	num4 = 0xff;
	num5 = 0xff;
	num6 = 0xff;
	num7 = 0xff;
	num8 = 0xfe;

	fp = fopen("./pass.5", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xff;
	num2 = 0xff;
	num3 = 0xff;
	num4 = 0xff;
	num5 = 0xff;
	num6 = 0xff;
	num7 = 0xff;
	num8 = 0xff;

	fp = fopen("./pass.6", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x00;

	fp = fopen("./pass.7", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x01;

	fp = fopen("./pass.8", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x02;

	fp = fopen("./pass.9", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x03;

	fp = fopen("./pass.10", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x04;

	fp = fopen("./pass.11", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x05;

	fp = fopen("./pass.12", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x06;

	fp = fopen("./pass.13", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x07;

	fp = fopen("./pass.14", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x08;

	fp = fopen("./pass.15", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x09;

	fp = fopen("./pass.16", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x0a;

	fp = fopen("./pass.17", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;
	num5 = 0x00;
	num6 = 0x00;
	num7 = 0x00;
	num8 = 0x0b;

	fp = fopen("./fail.2", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xff;
	num2 = 0xff;
	num3 = 0xff;
	num4 = 0xff;
	num5 = 0xff;
	num6 = 0xff;
	num7 = 0xff;
	num8 = 0xfd;

	fp = fopen("./fail.3", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fwrite(&num5, sizeof(uint8_t), 1, fp);
	fwrite(&num6, sizeof(uint8_t), 1, fp);
	fwrite(&num7, sizeof(uint8_t), 1, fp);
	fwrite(&num8, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xff;
	num2 = 0xff;
	num3 = 0xff;
	num4 = 0xff;

	fp = fopen("./pass.18", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x00;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;

	fp = fopen("./pass.19", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);
	
	num1 = 0x01;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;

	fp = fopen("./pass.20", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x02;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;

	fp = fopen("./pass.21", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x03;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;

	fp = fopen("./pass.22", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0xfe;
	num2 = 0xff;
	num3 = 0xff;
	num4 = 0xff;

	fp = fopen("./fail.4", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x04;
	num2 = 0x00;
	num3 = 0x00;
	num4 = 0x00;

	fp = fopen("./fail.5", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3, sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x0a;

	fp = fopen("./pass.23", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x0b;

	fp = fopen("./pass.24", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x0c;

	fp = fopen("./pass.25", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x0d;

	fp = fopen("./pass.26", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x0e;

	fp = fopen("./pass.27", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x0f;

	fp = fopen("./pass.28", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x10;

	fp = fopen("./fail.6", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);

	num1 = 0x09;

	fp = fopen("./fail.7", "wb");
	fwrite(&num1, sizeof(uint8_t), 1, fp);
	fclose(fp);


	return 0;

}
