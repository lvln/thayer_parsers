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

static int countEntries(FILE *fp) {
	int i = 0;
	uint8_t buf;
	
	while (fread(&buf, sizeof(uint8_t), 1, fp) > 0)
		i++;

	return i;
}

int main(void) {
	FILE *fp;
	uint8_t *nums;
	int len, i;

	if ((fp = fopen("./pass.1", "rb")) == NULL)
		exit(EXIT_FAILURE);
	
	len = countEntries(fp);

	fclose(fp);

	if ((nums = (uint8_t *)malloc(sizeof(uint8_t)*len)) == NULL)
		exit(EXIT_FAILURE);

	if ((fp = fopen("./pass.1", "rb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < len; i++) 
		fread(nums + i, sizeof(uint8_t), 1, fp);

	fclose(fp);

	if ((fp = fopen("./pass.3", "wb")) == NULL)
		exit(EXIT_FAILURE);


	for (i = 0; i < len; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);

	fclose(fp);

	nums[0] = 0x4D;
	nums[1] = 0x3C;
	
	if ((fp = fopen("./pass.4", "wb")) == NULL)
		exit(EXIT_FAILURE);


	for (i = 0; i < len; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);

	fclose(fp);
	
	if ((fp = fopen("./fail.1", "wb")) == NULL)
		exit(EXIT_FAILURE);


	for (i = 0; i < len - 1; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);

	fclose(fp);

	nums[0] = 0x0D;
	
	if ((fp = fopen("./fail.2", "wb")) == NULL)
		exit(EXIT_FAILURE);


	for (i = 0; i < len; i++)
		fwrite(nums + i, sizeof(uint8_t), 1 ,fp);

	fclose(fp);
	
	
	/*
	
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
	*/
	exit(EXIT_SUCCESS);

}
