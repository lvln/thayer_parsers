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
#include <stdbool.h>
#include <mavlink.h>

static void writeSPFile(message_t *mess, FILE *fp, int len) {
	int i;
	uint8_t buf;

	i = 0;
	
	while (i < len) {
		if (i < 4) buf = mess->header.timeS[i];
		else if (i < 8) buf = mess->header.timeUNS[i - 4];
		else if (i < 12) buf = mess->header.capturedPacketLength[i - 8];
		else if (i < 16) buf = mess->header.originalPacketLength[i - 12];
		else if (i < 20) buf = mess->header.messageFam[i - 16];
		else if (i == 20) buf = mess->header.iphl;
		else if (i == 21) buf = mess->header.dsf;
		else if (i < 24) buf = mess->header.totalLength[i - 22];
		else if (i < 26) buf = mess->header.id[i - 24];
		else if (i < 28) buf = mess->header.ffOffset[i - 26];
		else if (i == 28) buf = mess->header.ttl;
		else if (i == 29) buf = mess->header.protocol;
		else if (i < 32) buf = mess->header.headerChecksum[i - 30];
		else if (i < 36) buf = mess->header.sourceAddr[i - 32];
		else if (i < 40) buf = mess->header.destAddr[i - 36];
		else if (i < 42) buf = mess->header.sourcePort[i - 40];
		else if (i < 44) buf = mess->header.destPort[i - 42];
		else if (i < 46) buf = mess->header.length[i - 44];
		else if (i < 48) buf = mess->header.checkSum[i - 46];
		else if (i == 48) buf = mess->body.mavCode;
		else if (i == 49) buf =	mess->body.payloadLen;
		else if (i == 50) buf = mess->body.incompFlag;
		else if (i == 51) buf = mess->body.compFlag;
		else if (i == 52) buf = mess->body.packetSeq;
		else if (i == 53) buf = mess->body.systemID;
		else if (i == 54) buf = mess->body.compID;
		else if (i < 58) buf = mess->body.messageID[i - 55];		
		else if (i < 58 + 16) buf = mess->body.payload[i - 58];
		else if (i < 60 + 16) buf = mess->body.crc[i - 58 - 16];
		else buf = 0x21;
		
		i++;

		fwrite(&buf, sizeof(buf), 1, fp);

	}
}

int main(void) {
	FILE *fp;
	messArr_t *messArr;
	uint8_t old;
	int i;
	char fname[10];
	
	if ((fp = fopen("pass.2", "rb")) == NULL)
		exit(EXIT_FAILURE);

	messArr = readFile(fp);

	fclose(fp);

	if ((fp = fopen("./fail.2", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = messArr->messages[0].body.mavCode;
	messArr->messages[0].body.mavCode = 0x00;
	
	writeToFile(messArr->messages[0], fp);
	
	fclose(fp);

	messArr->messages[0].body.mavCode = old;
	
	if ((fp = fopen("./fail.3", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = messArr->messages[0].body.messageID[0];
	messArr->messages[0].body.messageID[0] = 0x21;
	
	writeToFile(messArr->messages[0], fp);
	
	fclose(fp);

	messArr->messages[0].body.messageID[0] = old;

	if ((fp = fopen("./fail.4", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = messArr->messages[0].body.payloadLen;
	messArr->messages[0].body.payloadLen = 0x21;
	
	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16);
	
	fclose(fp);

	messArr->messages[0].body.payloadLen = old;

	if ((fp = fopen("./fail.5", "wb")) == NULL)
		exit(EXIT_FAILURE);

	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16 - 1);
	
	fclose(fp);
	
	if ((fp = fopen("./fail.6", "wb")) == NULL)
		exit(EXIT_FAILURE);

	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16 + 1);
	
	fclose(fp);

	if ((fp = fopen("./fail.7", "wb")) == NULL)
		exit(EXIT_FAILURE);
	
	old = messArr->messages[0].header.messageFam[0];
	messArr->messages[0].header.messageFam[0] = 0x03;
	
	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16);
	
	fclose(fp);
	
	messArr->messages[0].header.messageFam[0] = old;
	
	if ((fp = fopen("./fail.8", "wb")) == NULL)
		exit(EXIT_FAILURE);
	
	old = messArr->messages[0].header.iphl;
	messArr->messages[0].header.iphl = 0x03;
	
	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16);
	
	fclose(fp);

	messArr->messages[0].header.iphl = old;

	if ((fp = fopen("./fail.9", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = messArr->messages[0].header.totalLength[0];
	messArr->messages[0].header.totalLength[0] = 0x03;
	
	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16);
	
	fclose(fp);

	messArr->messages[0].header.totalLength[0] = old;

	if ((fp = fopen("./fail.10", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = messArr->messages[0].header.ttl;
	messArr->messages[0].header.ttl = 0x03;
	
	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16);
	
	fclose(fp);

	messArr->messages[0].header.ttl = old;

	if ((fp = fopen("./fail.11", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = messArr->messages[0].header.sourcePort[0];
	messArr->messages[0].header.sourcePort[0] = 0x03;
	
	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16);
	
	fclose(fp);

	messArr->messages[0].header.sourcePort[0] = old;

	if ((fp = fopen("./fail.12", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = messArr->messages[0].header.destPort[0];
	messArr->messages[0].header.destPort[0] = 0x03;
	
	writeSPFile(messArr->messages, fp, toInt24le(messArr->messages[0].header.capturedPacketLength) + 16);
	
	fclose(fp);

	messArr->messages[0].header.destPort[0] = old;

	for (i = 0; i < 16; i++) {
		sprintf(fname, "./pass.%d", i + 5);

		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);

		old = messArr->messages[0].body.payload[i];
		messArr->messages[0].body.payload[i] = old + 0x01;
		
		writeToFile(messArr->messages[0], fp);

		fclose(fp);

		messArr->messages[0].body.payload[i] = old;
	}
	
	freeMem(messArr);

	if ((fp = fopen("pass.4", "rb")) == NULL)
		exit(EXIT_FAILURE);

	messArr = readFile(fp);

	fclose(fp);

	if ((fp = fopen("fail.13", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < messArr->n - 1; i++)
		writeToFile(messArr->messages[i], fp);

	writeSPFile(&(messArr->messages[i]), fp, toInt24le(messArr->messages[i].header.capturedPacketLength) + 16 - 1);

	fclose(fp);

	if ((fp = fopen("fail.14", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < messArr->n - 1; i++)
		writeToFile(messArr->messages[i], fp);

	writeSPFile(&(messArr->messages[i]), fp, toInt24le(messArr->messages[i].header.capturedPacketLength) + 16 + 1);

	fclose(fp);

	freeMem(messArr);
	
	exit(EXIT_SUCCESS);

}
