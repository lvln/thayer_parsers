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
	uint8_t byte;

	fp = fopen("./pass.1", "wb");
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./pass.2", "wb");
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./pass.3", "wb");
	byte = 0xfe;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0x63;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0xff;
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./pass.4", "wb");
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./pass.5", "wb");
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./pass.6", "wb");
	byte = 0xfe;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0xef;
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./fail.1", "wb");
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'c';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./fail.2", "wb");
	byte = 0x00;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./fail.3", "wb");
	byte = 0xfe;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0x63;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0xff;
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 0xff;
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./fail.4", "wb");
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./fail.5", "wb");
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'c';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	byte = 'a';
	fwrite(&byte, sizeof(byte), 1, fp);
	fclose(fp);

	fp = fopen("./fail.6", "wb");
	fclose(fp);

	exit(EXIT_SUCCESS);
}
