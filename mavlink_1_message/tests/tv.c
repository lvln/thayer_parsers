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

typedef struct {
	uint8_t timestamp[4];
	uint8_t timestampUsNs[4];
	uint8_t capPacketLen[4];
	uint8_t origPacketLen[4];
	uint8_t messFam[4];
	uint8_t ipHl;
	uint8_t dsf;
	uint8_t totalLen[2];
	uint8_t id[2];
	uint8_t ffo[2];
	uint8_t ttl;
	uint8_t protocol;
	uint8_t hdrChksum[2];
	uint8_t srcAddr[4];
	uint8_t destAddr[4];
	uint8_t srcPort[2];
	uint8_t destPort[2];
	uint8_t len[2];
	uint8_t chksum[2];
	uint8_t version;
	uint8_t payloadLen;
	uint8_t incompFlag;
	uint8_t compFlag;
	uint8_t packetSeq;
	uint8_t sysID;
	uint8_t compID;
	uint8_t msgID[3];
	uint8_t time[4];
	uint8_t pressAbs[4];
	uint8_t pressDiff[4];
	uint8_t temp[2];
	uint8_t tempPressDiff[2];
	uint8_t crc[2];
} spPayload_t;

static spPayload_t fillPayload(FILE *fp, int *len) {
	// Variable declarations.
	spPayload_t payload;
	int i;
	uint8_t buf;
	
	i = 0;
	
	while (fread(&buf, sizeof(uint8_t), 1, fp) == 1) {
		if (i < 4) payload.timestamp[i] = buf;
		else if (i < 8) payload.timestampUsNs[i - 4] = buf;
		else if (i < 12) payload.capPacketLen[i - 8] = buf;
		else if (i < 16) payload.origPacketLen[i - 12] = buf;
		else if (i < 20) payload.messFam[i - 16] = buf;
		else if (i == 20) payload.ipHl = buf;
		else if (i == 21) payload.dsf = buf;
		else if (i < 24) payload.totalLen[i - 22] = buf;
		else if (i < 26) payload.id[i - 24] = buf;
		else if (i < 28) payload.ffo[i - 26] = buf;
		else if (i == 28) payload.ttl = buf;
		else if (i == 29) payload.protocol = buf;
		else if (i < 32) payload.hdrChksum[i - 30] = buf;
		else if (i < 36) payload.srcAddr[i - 32] = buf;
		else if (i < 40) payload.destAddr[i - 36] = buf;
		else if (i < 42) payload.srcPort[i - 40] = buf;
		else if (i < 44) payload.destPort[i - 42] = buf;
		else if (i < 46) payload.len[i - 44] = buf;
		else if (i < 48) payload.chksum[i - 46] = buf;
		else if (i == 48) payload.version = buf;
		else if (i == 49) payload.payloadLen = buf;
		else if (i == 50) payload.incompFlag = buf;
		else if (i == 51) payload.compFlag = buf;
		else if (i == 52) payload.packetSeq = buf;
		else if (i == 53) payload.sysID = buf;
		else if (i == 54) payload.compID = buf;
		else if (i < 58) payload.msgID[i - 55] = buf;
		else if (i < 62) payload.time[i - 58] = buf;
		else if (i < 66) payload.pressAbs[i - 62] = buf;
		else if (i < 70) payload.pressDiff[i - 66] = buf;
		else if (i < 72) payload.temp[i - 70] = buf;
		else if (i < 74) payload.tempPressDiff[i - 72] = buf;
		else if (i < 76) payload.crc[i - 74] = buf;
		i++;

	}

	*len = i;
	
	return payload;
}

static void writeFile(FILE *ofile, spPayload_t payload, int len) {
	// Variable declarations.
	int i;
	uint8_t buf;

	i = 0;

	while (i < len) {
		if (i < 4) buf = payload.timestamp[i];
		else if (i < 8) buf = payload.timestampUsNs[i - 4];
		else if (i < 12) buf = payload.capPacketLen[i - 8];
		else if (i < 16) buf = payload.origPacketLen[i - 12];
		else if (i < 20) buf = payload.messFam[i - 16];
		else if (i == 20) buf = payload.ipHl;
		else if (i == 21) buf = payload.dsf;
		else if (i < 24) buf = payload.totalLen[i - 22];
		else if (i < 26) buf = payload.id[i - 24];
		else if (i < 28) buf = payload.ffo[i - 26];
		else if (i == 28) buf = payload.ttl;
		else if (i == 29) buf = payload.protocol;
		else if (i < 32) buf = payload.hdrChksum[i - 30];
		else if (i < 36) buf = payload.srcAddr[i - 32];
		else if (i < 40) buf = payload.destAddr[i - 36];
		else if (i < 42) buf = payload.srcPort[i - 40];
		else if (i < 44) buf = payload.destPort[i - 42];
		else if (i < 46) buf = payload.len[i - 44];
		else if (i < 48) buf = payload.chksum[i - 46];
		else if (i == 48) buf = payload.version;
		else if (i == 49) buf = payload.payloadLen;
		else if (i == 50) buf = payload.incompFlag;
		else if (i == 51) buf = payload.compFlag;
		else if (i == 52) buf = payload.packetSeq;
		else if (i == 53) buf = payload.sysID;
		else if (i == 54) buf = payload.compID;
		else if (i < 58) buf = payload.msgID[i - 55];
		else if (i < 62) buf = payload.time[i - 58];
		else if (i < 66) buf = payload.pressAbs[i - 62];
		else if (i < 70) buf = payload.pressDiff[i - 66];
		else if (i < 72) buf = payload.temp[i - 70];
		else if (i < 74) buf = payload.tempPressDiff[i - 72];
		else if (i < 76) buf = payload.crc[i - 74];
		i++;

		fwrite(&buf, sizeof(buf), 1, ofile);

	}

}

int main(void) {
	FILE *fp;
	spPayload_t mess;
	int len;
	uint8_t old;
	
	if ((fp = fopen("./pass.2", "rb")) == NULL)
		exit(EXIT_FAILURE);
	
	mess = fillPayload(fp, &len);

	fclose(fp);

	if ((fp = fopen("./fail.2", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = mess.version;
	mess.version = 0x00;
	
	writeFile(fp, mess, len);
	
	fclose(fp);

	mess.version = old;

	if ((fp = fopen("./fail.3", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = mess.msgID[0];
	mess.msgID[0] = 0x21;
	
	writeFile(fp, mess, len);
	
	fclose(fp);

	mess.msgID[0] = old;

	if ((fp = fopen("./fail.4", "wb")) == NULL)
		exit(EXIT_FAILURE);

	old = mess.payloadLen;
	mess.payloadLen = 0x21;
	
	writeFile(fp, mess, len);
	
	fclose(fp);

	mess.payloadLen = old;

	if ((fp = fopen("./fail.5", "wb")) == NULL)
		exit(EXIT_FAILURE);

	writeFile(fp, mess, len - 1);
	
	fclose(fp);

		if ((fp = fopen("./fail.6", "wb")) == NULL)
		exit(EXIT_FAILURE);

	writeFile(fp, mess, len + 1);
	
	fclose(fp);

	exit(EXIT_SUCCESS);

}
