/* 
 * tv.c --- 
 * 
1;95;0c * Author: Stephen Taylor, Brandon Guzman, Josh Meise
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
#include <mavlink.h>

static void writeEditedFile(message_t *mess, FILE *fp, int len, int payload) {
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
		else if (i < 58 + payload) buf = mess->body.payload[i - 58];
		else if (i < 60 + payload) buf = mess->body.crc[i - 58 - payload];
		else buf = 0x21;
		
		i++;

		fwrite(&buf, sizeof(buf), 1, fp);

	}
}

int main(void) {
	FILE *fp;
	messArr_t *messArr;
	int num, i, j, prevNum;
	uint8_t old;
	char fname[10];
	
	if ((fp = fopen("./pass.6", "rb")) == NULL)
		exit(EXIT_FAILURE);
	
	messArr = readFile(fp);

	fclose(fp);


	prevNum = 8;
	
	for (j = 0; j < 2; j++) {
		sprintf(fname, "./fail.%d", 2 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
	
		old = messArr->messages[j].body.mavCode;
		messArr->messages[j].body.mavCode = 0x00;
	
		writeToFile(messArr->messages[j], fp);
		
		fclose(fp);
		
		messArr->messages[j].body.mavCode = old;

		sprintf(fname, "./fail.%d", 3 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[j].body.messageID[0];
		messArr->messages[j].body.messageID[0] =  0x01;
	
		writeToFile(messArr->messages[j], fp);
		
		fclose(fp);
		
		messArr->messages[j].body.messageID[0] = old;

		sprintf(fname, "./fail.%d", 4 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[j].body.payloadLen;
		messArr->messages[j].body.payloadLen = 0x21;

		writeEditedFile(&(messArr->messages[j]), fp, toInt24le(messArr->messages[j].header.capturedPacketLength) + 16, toInt16be(messArr->messages[j].header.totalLength) - 40);
		
		fclose(fp);
		
		messArr->messages[j].body.payloadLen = old;
		
		sprintf(fname, "./fail.%d", 5 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		writeEditedFile(&(messArr->messages[j]), fp, toInt24le(messArr->messages[j].header.capturedPacketLength) + 16 - 1, toInt16be(messArr->messages[j].header.totalLength) - 40);
	
		fclose(fp);

		sprintf(fname, "./fail.%d", 6 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);

		writeEditedFile(&(messArr->messages[j]), fp, toInt24le(messArr->messages[j].header.capturedPacketLength) + 16 + 1, toInt16be(messArr->messages[j].header.totalLength) - 40);
	
		fclose(fp);

		sprintf(fname, "./fail.%d", 7 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[j].header.messageFam[0];
		messArr->messages[j].header.messageFam[0] = 0x03;
	
		writeToFile(messArr->messages[j], fp);
	
		fclose(fp);
		
		messArr->messages[j].header.messageFam[0] = old;

		sprintf(fname, "./fail.%d", 8 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[j].header.iphl;
		messArr->messages[j].header.iphl = 0x03;

		writeToFile(messArr->messages[j], fp);
		fclose(fp);
		
		messArr->messages[j].header.iphl = old;

		sprintf(fname, "./fail.%d", 9 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);

		old = messArr->messages[j].header.ttl;
		messArr->messages[j].header.ttl = 0x03;
		
		writeToFile(messArr->messages[j], fp);
		
		fclose(fp);
		
		messArr->messages[j].header.ttl = old;

		sprintf(fname, "./fail.%d", 10 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[j].header.sourcePort[0];
		messArr->messages[j].header.sourcePort[0] = 0x03;
		
		writeToFile(messArr->messages[j], fp);
	
		fclose(fp);
		
		messArr->messages[j].header.sourcePort[0] = old;

		sprintf(fname, "./fail.%d", 11 + 10*j);
		if ((fp = fopen(fname, "wb")) == NULL)
			exit(EXIT_FAILURE);
		
		old = messArr->messages[j].header.destPort[0];
		messArr->messages[j].header.destPort[0] = 0x03;
		
		writeToFile(messArr->messages[j], fp);
		
		fclose(fp);

		messArr->messages[j].header.destPort[0] = old;
		
		for (i = 0; i < messArr->messages[j].body.payloadLen; i++) {
			num = prevNum + i;
			sprintf(fname, "./pass.%d", num);
			
			if ((fp = fopen(fname, "wb")) == NULL)
				exit(EXIT_FAILURE);
			
			old = messArr->messages[j].body.payload[i];
			messArr->messages[j].body.payload[i] = old + 0x01;
			
			writeToFile(messArr->messages[j], fp);
			
			fclose(fp);
			
			messArr->messages[j].body.payload[i] = old;
		}

		prevNum = ++num;
	}
	
	freeMem(messArr);

	if ((fp = fopen("pass.9", "rb")) == NULL)
		exit(EXIT_FAILURE);

	messArr = readFile(fp);

	if ((fp = fopen("fail.22", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < messArr->n - 1; i++)
		writeToFile(messArr->messages[i], fp);

	writeEditedFile(&(messArr->messages[i]), fp, toInt24le(messArr->messages[i].header.capturedPacketLength) + 16 - 1, messArr->messages[i].body.payloadLen);

	fclose(fp);

	if ((fp = fopen("fail.23", "wb")) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; i < messArr->n - 1; i++)
		writeToFile(messArr->messages[i], fp);

	writeEditedFile(&(messArr->messages[i]), fp, toInt24le(messArr->messages[i].header.capturedPacketLength) + 16 + 1, messArr->messages[i].body.payloadLen);

	fclose(fp);

	freeMem(messArr);

	exit(EXIT_SUCCESS);

}
