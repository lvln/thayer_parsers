/* 
 * tvshort.c --- changes various fields to generate fail test cases
 * 
 * Author: Joshua M. Meise
 * Created: 08-02-2024
 * Version: 1.0
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mavlink.c>

int main(void) {
	// Variable declarations
	FILE *fp;
	mavMessArr_t *message;
	uint8_t old;

	if ((fp = fopen("pass.3", "rb")) == NULL) {
		printf("File not opened successfully.");
		exit(EXIT_FAILURE);
	}

	message = readFileMav(fp);

	fclose(fp);

	if ((fp = fopen("fail.1", "wb")) == NULL) {
		printf("File not opened successfully.\n");
		exit(EXIT_FAILURE);
	}

	old = message->messages[0].mavCode;
	message->messages[0].mavCode = 0x3e;

	writeMessageToFileMav(message->messages[0], fp);

	fclose(fp);

	message->messages[0].mavCode = old;

	if ((fp = fopen("fail.2", "wb")) == NULL) {
		printf("File not opened successfully.\n");
		exit(EXIT_FAILURE);
	}

	old = message->messages[0].payloadLen;
	message->messages[0].payloadLen = 0x3e;

	writeMessageToFileMav(message->messages[0], fp);

	fclose(fp);

	message->messages[0].payloadLen = old;

	if ((fp = fopen("fail.3", "wb")) == NULL) {
		printf("File not opened successfully.\n");
		exit(EXIT_FAILURE);
	}

	old = message->messages[0].incompFlag;
	message->messages[0].incompFlag = 0x3e;

	writeMessageToFileMav(message->messages[0], fp);

	fclose(fp);

	message->messages[0].incompFlag = old;

	if ((fp = fopen("fail.4", "wb")) == NULL) {
		printf("File not opened successfully.\n");
		exit(EXIT_FAILURE);
	}

	old = message->messages[0].compFlag;
	message->messages[0].compFlag = 0x3e;

	writeMessageToFileMav(message->messages[0], fp);

	fclose(fp);

	message->messages[0].compFlag = old;
	
	if ((fp = fopen("fail.5", "wb")) == NULL) {
		printf("File not opened successfully.\n");
		exit(EXIT_FAILURE);
	}

	old = message->messages[0].messageID[0];
	message->messages[0].messageID[0] = 0x02;

	writeMessageToFileMav(message->messages[0], fp);

	fclose(fp);

	message->messages[0].messageID[0] = old;

	exit(EXIT_SUCCESS);
}
