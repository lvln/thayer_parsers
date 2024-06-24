/* 
 * tv.c --- 
 * 
 * Author: Stephen Taylor, Brandon Guzman, Josh Meise
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

#define LEN 24

int main(void) {
	FILE *fp;
	uint8_t nums[LEN];
	int i;
	
	nums[0] = 0xD4;
	nums[1] = 0xC3;
	nums[2] = 0xB2;
	nums[3] = 0xA1;
	nums[4] = 0x02;
	nums[5] = 0x00;
	nums[6] = 0x04;
	nums[7] = 0x00;
	nums[8] = 0x00;
	nums[9] = 0x00;
	nums[10] = 0x00;
	nums[11] = 0x00;
	nums[12] = 0x00;
	nums[13] = 0x00;
	nums[14] = 0x00;
	nums[15] = 0x00;
	nums[16] = 0x32;
	nums[17] = 0x08;
	nums[18] = 0x00;
	nums[19] = 0x00;
	nums[20] = 0x00;
	nums[21] = 0x00;
	nums[22] = 0x00;
	nums[23] = 0x00;
	
	if ((fp = fopen("./pass.1", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < LEN; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);

	fclose(fp);

	nums[0] = 0x4D;
	nums[1] = 0x3C;
	
	if ((fp = fopen("./pass.2", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < LEN; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);

	fclose(fp);
	
	nums[0] = 0x44;
	
	if ((fp = fopen("./fail.1", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < LEN; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);

	fclose(fp);

	nums[0] = 0x4D;
	
	if ((fp = fopen("./fail.2", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < LEN - 1; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);
	
	fclose(fp);

	if ((fp = fopen("./fail.3", "wb")) == NULL)
		exit(EXIT_FAILURE);

	fclose(fp);

	exit(EXIT_SUCCESS);

}
