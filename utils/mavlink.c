/* 
 * mavlink.c --- 
 * 
 * Author: Joshua M. Meise
 * Created: 06-27-2024
 * Version: 1.0
 * 
 * Description: 
 * 
 */

// Library inclusions.
#include <stdio.h>
#include <mavlink.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * Convert from 8 bit hexadecimal to integer.
 * Inputs: hex number.
 * Outputs: integer enquvalent.
 */
int toInt8(uint8_t *arr) {
	return (int)arr[0];
}

/*
 * Convert from 16 bit hexadecimal (little endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt16le(uint8_t *arr) {
	return (int)arr[0] + (int)arr[1]*256;
}

/*
 * Convert from 16 bit hexadecimal (big endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt16be(uint8_t *arr) {
	return (int)arr[1] + (int)arr[0]*256;
}

/*
 * Convert from 24 bit hexadecimal (little endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt24le(uint8_t *arr) {
	return (int)arr[0] + (int)arr[1]*256 + (int)arr[2]*4096;
}

/*
 * Convert from 24 bit hexadecimal (big endian) to integer.
 * Inputs: array containing hex digits.
 * Outputs: integer enquvalent.
 */
int toInt24be(uint8_t *arr) {
	return (int)arr[2] + (int)arr[1]*256 + (int)arr[0]*4096;
}

/*
 * Writes a pcap header to a file.
 * Inputs: pcap file data structure, file to which to write.
 * Outputs: none.
 */	
void writeHeader(pcap_t *pcapFile, FILE *ofile) {
	// Variable declarations.
	int i;

	for (i = 0; i < 24; i++) 
		if (fwrite(&(pcapFile->header[i]), sizeof(uint8_t), 1, ofile) != 1)
			printf("Problem writing to file.\n");

}

/*
 * Write a raw MAVLink message to a file - just header and payload.
 * Inputs: message to write, file pointer.
 * Outputs: none.
 */
void writeMessageToFile(message_t mess, FILE *ofile) {
	// Variable declarations.
	int i, payload;
	uint8_t buf;
	
	payload = toInt8(&mess.body.payloadLen);

	for (i = 0; i < payload + 12; i++) {
		if (i == 0) buf = mess.body.mavCode;
		else if (i == 1) buf =	mess.body.payloadLen;
		else if (i == 2) buf = mess.body.incompFlag;
		else if (i == 3) buf = mess.body.compFlag;
		else if (i == 4) buf = mess.body.packetSeq;
		else if (i == 5) buf = mess.body.systemID;
		else if (i == 6) buf = mess.body.compID;
		else if (i < 10) buf = mess.body.messageID[i - 7];		
		else if (i < 10 + payload) buf = mess.body.payload[i - 10];
		else buf = mess.body.crc[i - 10 - payload];

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Error writing byte %02x to file.\n", buf);
	}

}	
								 
/*
 * Write a AMVLink message to a file.
 * Inputs: message to write, file pointer.
 * Outputs: none.
 */
void writeToFile(message_t mess, FILE *ofile) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;

	len = toInt24le(mess.header.capturedPacketLength) + 16;
	payload = toInt8(&mess.body.payloadLen);
									 
	for (i = 0; i < len; i++) {
		if (i < 4) buf = mess.header.timeS[i];
		else if (i < 8) buf = mess.header.timeUNS[i - 4];
		else if (i < 12) buf = mess.header.capturedPacketLength[i - 8];
		else if (i < 16) buf = mess.header.originalPacketLength[i - 12];
		else if (i < 20) buf = mess.header.messageFam[i - 16];
		else if (i == 20) buf = mess.header.iphl;
		else if (i == 21) buf = mess.header.dsf;
		else if (i < 24) buf = mess.header.totalLength[i - 22];
		else if (i < 26) buf = mess.header.id[i - 24];
		else if (i < 28) buf = mess.header.ffOffset[i - 26];
		else if (i == 28) buf = mess.header.ttl;
		else if (i == 29) buf = mess.header.protocol;
		else if (i < 32) buf = mess.header.headerChecksum[i - 30];
		else if (i < 36) buf = mess.header.sourceAddr[i - 32];
		else if (i < 40) buf = mess.header.destAddr[i - 36];
		else if (i < 42) buf = mess.header.sourcePort[i - 40];
		else if (i < 44) buf = mess.header.destPort[i - 42];
		else if (i < 46) buf = mess.header.length[i - 44];
		else if (i < 48) buf = mess.header.checkSum[i - 46];
		else if (i == 48) buf = mess.body.mavCode;
		else if (i == 49) buf =	mess.body.payloadLen;
		else if (i == 50) buf = mess.body.incompFlag;
		else if (i == 51) buf = mess.body.compFlag;
		else if (i == 52) buf = mess.body.packetSeq;
		else if (i == 53) buf = mess.body.systemID;
		else if (i == 54) buf = mess.body.compID;
		else if (i < 58) buf = mess.body.messageID[i - 55];		
		else if (i < 58 + payload) buf = mess.body.payload[i - 58];
		else buf = mess.body.crc[i - 58 - payload];

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Error writing byte %02x to file.\n", buf);
	}

}

/*
 * Print out a message.
 * Inputs: message to print
 * Outputs: none.
 */
void printMessage(message_t mess) {
	// Variable declarations.
	int i, len, payload;
	uint8_t buf;

	len = toInt24le(mess.header.capturedPacketLength) + 16;
	payload = toInt8(&mess.body.payloadLen);
									 
	printf("Message\n" );
	
	for (i = 0; i < len; i++) {
		if (i < 4) buf = mess.header.timeS[i];
		else if (i < 8) buf = mess.header.timeUNS[i - 4];
		else if (i < 12) buf = mess.header.capturedPacketLength[i - 8];
		else if (i < 16) buf = mess.header.originalPacketLength[i - 12];
		else if (i < 20) buf = mess.header.messageFam[i - 16];
		else if (i == 20) buf = mess.header.iphl;
		else if (i == 21) buf = mess.header.dsf;
		else if (i < 24) buf = mess.header.totalLength[i - 22];
		else if (i < 26) buf = mess.header.id[i - 24];
		else if (i < 28) buf = mess.header.ffOffset[i - 26];
		else if (i == 28) buf = mess.header.ttl;
		else if (i == 29) buf = mess.header.protocol;
		else if (i < 32) buf = mess.header.headerChecksum[i - 30];
		else if (i < 36) buf = mess.header.sourceAddr[i - 32];
		else if (i < 40) buf = mess.header.destAddr[i - 36];
		else if (i < 42) buf = mess.header.sourcePort[i - 40];
		else if (i < 44) buf = mess.header.destPort[i - 42];
		else if (i < 46) buf = mess.header.length[i - 44];
		else if (i < 48) buf = mess.header.checkSum[i - 46];
		else if (i == 48) buf = mess.body.mavCode;
		else if (i == 49) buf =	mess.body.payloadLen;
		else if (i == 50) buf = mess.body.incompFlag;
		else if (i == 51) buf = mess.body.compFlag;
		else if (i == 52) buf = mess.body.packetSeq;
		else if (i == 53) buf = mess.body.systemID;
		else if (i == 54) buf = mess.body.compID;
		else if (i < 58) buf = mess.body.messageID[i - 55];		
		else if (i < 58 + payload) buf = mess.body.payload[i - 58];
		else buf = mess.body.crc[i - 58 - payload];

		printf("%02x ", buf);
	}

	printf("\n");
}

/*
 * Re-inserts the trailing zeros in a MAVLink message to standardise the format for parsing.
 * Inputs: message
 * Outputs: updated message
 */
void untruncate(message_t *mess) {
	// Variable declarations.
	int payload, messageCode, bytesToAdd, len, i, intNum;
	
	messageCode = toInt24le((*mess).body.messageID);
	payload = toInt8(&(*mess).body.payloadLen);

	if (messageCode == HEARTBEAT)
		len = HEARTBEAT_LEN;
	else if (messageCode == SYS_STATUS)
		len = SYS_STATUS_LEN;
	else if (messageCode == PING)
		len = PING_LEN;
	else if (messageCode == LINK_NODE_STATUS)
		len = LINK_NODE_STATUS_LEN;
	else if (messageCode == PARAM_VALUE)
		len = PARAM_VALUE_LEN;
	else if (messageCode == GPS_RAW_INT)
		len = GPS_RAW_INT_LEN;
	else if (messageCode == SCALED_PRESSURE)
		len = SCALED_PRESSURE_LEN;
	else if (messageCode == ATTITUDE)
		len = ATTITUDE_LEN;
	else if (messageCode == ATTITUDE_QUATERNION)
		len = ATTITUDE_QUATERNION_LEN;
	else if (messageCode == LOCAL_POSITION_NED)
		len = LOCAL_POSITION_NED_LEN;
	else if (messageCode == GLOBAL_POSITION_INT)
		len = GLOBAL_POSITION_INT_LEN;
	else if (messageCode == MISSION_CURRENT)
		len = MISSION_CURRENT_LEN;
	else if (messageCode == MISSION_ITEM_REACHED)
		len = MISSION_ITEM_REACHED_LEN;
	else if (messageCode == RC_CHANNELS)
		len = RC_CHANNELS_LEN;
	else if (messageCode == VFR_HUD)
		len = VFR_HUD_LEN;
	else if (messageCode == COMMAND_ACK)
		len = COMMAND_ACK_LEN;
	else if (messageCode == ATTITUDE_TARGET)
		len = ATTITUDE_TARGET_LEN;
	else if (messageCode == POSITION_TARGET_LOCAL_NED)
		len = POSITION_TARGET_LOCAL_NED_LEN;
	else if (messageCode == POSITION_TARGET_GLOBAL_INT)
		len = POSITION_TARGET_GLOBAL_INT_LEN;
	else if (messageCode == RADIO_STATUS)
		len = RADIO_STATUS_LEN;
	else if (messageCode == ALTITUDE)
		len = ALTITUDE_LEN;
	else if (messageCode == BATTERY_STATUS)
		len = BATTERY_STATUS_LEN;
	else if (messageCode == ESTIMATOR_STATUS)
		len = ESTIMATOR_STATUS_LEN;
	else if (messageCode == VIBRATION)
		len = VIBRATION_LEN;
	else if (messageCode == HOME_POSITION)
		len = HOME_POSITION_LEN;
	else if (messageCode == EXTENDED_SYSSTATE)
		len = EXTENDED_SYSSTATE_LEN;
	else if (messageCode == STATUSTEXT)
		len = STATUSTEXT_LEN;
	else if (messageCode == UTM_GLOBAL_POSITION)
		len = UTM_GLOBAL_POSITION_LEN;
	else if (messageCode == TIME_ESTIMATE_TO_TARGET)
		len = TIME_ESTIMATE_TO_TARGET_LEN;
	else if (messageCode == EVENT)
		len = EVENT_LEN;
	else if (messageCode == CURRENT_EVENT_SEQUENCE)
		len = CURRENT_EVENT_SEQUENCE_LEN;
	else if (messageCode == OPEN_DRONE_ID_LOCATION)
		len = OPEN_DRONE_ID_LOCATION_LEN;
	else if (messageCode == OPEN_DRONE_ID_SYSTEM)
		len = OPEN_DRONE_ID_SYSTEM_LEN;
	else
		printf("Invalid message code %d.\n", messageCode);

	if (len != payload) {
		bytesToAdd = len - payload;
		
		intNum = toInt24le((*mess).header.capturedPacketLength) + bytesToAdd;
		for (i = 0; i < 4; i++)
			(*mess).header.capturedPacketLength[i] = (uint8_t)(intNum >> (i * 8));
		
		
		intNum = toInt24le((*mess).header.originalPacketLength) + bytesToAdd;
		for (i = 0; i < 4; i++)
			(*mess).header.originalPacketLength[i] = (uint8_t)(intNum >> (i * 8));
		
		intNum = toInt16be((*mess).header.totalLength) + bytesToAdd;
		for (i = 1; i >= 0; i--)
			(*mess).header.totalLength[i] = (uint8_t)(intNum >> ((i - 1) * 8));
		
		intNum = toInt16be((*mess).header.length) + bytesToAdd;
		for (i = 1; i >= 0; i--)
			(*mess).header.length[i] = (uint8_t)(intNum >> ((i - 1) * 8));

		intNum = toInt8(&(*mess).body.payloadLen) + bytesToAdd;
		(*mess).body.payloadLen = (uint8_t)intNum;

		//INcrease memory for payload
		(*mess).body.payload = (uint8_t *)realloc((*mess).body.payload, sizeof(uint8_t)*(*mess).body.payloadLen);

		// Fill the additional memory with 0s.
		for (i = payload; i < (*mess).body.payloadLen; i++)
			(*mess).body.payload[i] = 0x00;
		
	}
	
}

/*
 * Read a file that contains just MAVLink messagess - no pcap header and no UDP headers.
 * Inputs: input file
 * Outputs: array of messages; NULL if unsuccessful
 */
messArr_t *readMavFile(FILE *ifile) {
	// variable declarations.
	messArr_t *messArr;
	int i, payload, numMess, bytesRead;
	message_t mess;
	uint8_t buf;
	
	if ((messArr = (messArr_t *)malloc(sizeof(messArr_t))) == NULL)
		return NULL;

	i = 0;
	payload = 0;
	numMess = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		if (i == 0) mess.body.mavCode = buf;
		else if (i == 1) {
			mess.body.payloadLen = buf;

			payload = (int)buf;
			
			if ((mess.body.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL)
				printf("Memory allocation failed.\n");
		}
		else if (i == 2) mess.body.incompFlag = buf;
		else if (i == 3) mess.body.compFlag = buf;
		else if (i == 4) mess.body.packetSeq = buf;
		else if (i == 5) mess.body.systemID = buf;
		else if (i == 6) mess.body.compID = buf;
		else if (i < 10) mess.body.messageID[i - 7] = buf;		
		else if (i < 10 + payload) mess.body.payload[i - 10] = buf;
		else mess.body.crc[i - 10 - payload] = buf;		
		
		i++;

		// If the end of a message has been reached.
		if (i == (12 + payload) && payload != 0) {
			if (numMess == 0) 
				messArr->messages = (message_t *)malloc(sizeof(message_t));
			else 
				messArr->messages = (message_t *)realloc(messArr->messages, sizeof(message_t)*(numMess + 1));

			messArr->messages[numMess] = mess;
					
			// Reset all variables
			i = 0;
			payload = 0;
			numMess++;
		}
	}
	messArr->n = numMess;
	
	return messArr;
}


/*
 * Read a file that contains just messagess - no pcap header.
 * Inputs: input file
 * Outputs: array of messages; NULL if unsuccessful
 */
messArr_t *readFile(FILE *ifile) {
	// variable declarations.
	messArr_t *messArr;
	int i, payload, numMess, bytesRead;
	message_t mess;
	uint8_t buf;
	
	if ((messArr = (messArr_t *)malloc(sizeof(messArr_t))) == NULL)
		return NULL;

	i = 0;
	payload = 0;
	numMess = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		if (i < 4) mess.header.timeS[i] = buf;
		else if (i < 8) mess.header.timeUNS[i - 4] = buf;
		else if (i < 12) mess.header.capturedPacketLength[i - 8] = buf;
		else if (i < 16) mess.header.originalPacketLength[i - 12] = buf;
		else if (i < 20) mess.header.messageFam[i - 16] = buf;
		else if (i == 20) mess.header.iphl = buf;
		else if (i == 21) mess.header.dsf = buf;
		else if (i < 24) mess.header.totalLength[i - 22] = buf;
		else if (i < 26) mess.header.id[i - 24] = buf;
		else if (i < 28) mess.header.ffOffset[i - 26] = buf;
		else if (i == 28) mess.header.ttl = buf;
		else if (i == 29) mess.header.protocol = buf;
		else if (i < 32) mess.header.headerChecksum[i - 30] = buf;
		else if (i < 36) mess.header.sourceAddr[i - 32] = buf;
		else if (i < 40) mess.header.destAddr[i - 36] = buf;
		else if (i < 42) mess.header.sourcePort[i - 40] = buf;
		else if (i < 44) mess.header.destPort[i - 42] = buf;
		else if (i < 46) mess.header.length[i - 44] = buf;
		else if (i < 48) mess.header.checkSum[i - 46] = buf;
		else if (i == 48) mess.body.mavCode = buf;
		else if (i == 49) {
			mess.body.payloadLen = buf;

			payload = (int)buf;
			
			if ((mess.body.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL)
				printf("Memory allocation failed.\n");
		}
		else if (i == 50) mess.body.incompFlag = buf;
		else if (i == 51) mess.body.compFlag = buf;
		else if (i == 52) mess.body.packetSeq = buf;
		else if (i == 53) mess.body.systemID = buf;
		else if (i == 54) mess.body.compID = buf;
		else if (i < 58) mess.body.messageID[i - 55] = buf;		
		else if (i < 58 + payload) mess.body.payload[i - 58] = buf;
		else mess.body.crc[i - 58 - payload] = buf;		
		
		i++;

		// If the end of a message has been reached.
		if (i == (60 + payload) && payload != 0) {
			if (numMess == 0) 
				messArr->messages = (message_t *)malloc(sizeof(message_t));
			else 
				messArr->messages = (message_t *)realloc(messArr->messages, sizeof(message_t)*(numMess + 1));

			messArr->messages[numMess] = mess;
					
			// Reset all variables
			i = 0;
			payload = 0;
			numMess++;
		}
	}
	messArr->n = numMess;
	
	return messArr;
}

/*
 * Read a pcap file (header and all messages.
 * Inputs: input file
 * Outputs: pcap file data structure; NULL if unsuccessful
 */
pcap_t *readPcapFile(FILE *ifile) {
	// variable declarations.
	pcap_t *pcapFile;
	int i, payload, numMess, bytesRead;
	message_t mess;
	uint8_t buf;
	
	if ((pcapFile = (pcap_t *)malloc(sizeof(pcap_t))) == NULL)
		return NULL;

	i = 0;
	
	// Read the first 24 bytes of the file - file header.
	while(i < 24) {
		bytesRead = fread(&buf, sizeof(buf), 1, ifile);

		pcapFile->header[i] = buf;
				
		i++;
	}

	i = 0;
	payload = 0;
	numMess = 0;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		if (i < 4) mess.header.timeS[i] = buf;
		else if (i < 8) mess.header.timeUNS[i - 4] = buf;
		else if (i < 12) mess.header.capturedPacketLength[i - 8] = buf;
		else if (i < 16) mess.header.originalPacketLength[i - 12] = buf;
		else if (i < 20) mess.header.messageFam[i - 16] = buf;
		else if (i == 20) mess.header.iphl = buf;
		else if (i == 21) mess.header.dsf = buf;
		else if (i < 24) mess.header.totalLength[i - 22] = buf;
		else if (i < 26) mess.header.id[i - 24] = buf;
		else if (i < 28) mess.header.ffOffset[i - 26] = buf;
		else if (i == 28) mess.header.ttl = buf;
		else if (i == 29) mess.header.protocol = buf;
		else if (i < 32) mess.header.headerChecksum[i - 30] = buf;
		else if (i < 36) mess.header.sourceAddr[i - 32] = buf;
		else if (i < 40) mess.header.destAddr[i - 36] = buf;
		else if (i < 42) mess.header.sourcePort[i - 40] = buf;
		else if (i < 44) mess.header.destPort[i - 42] = buf;
		else if (i < 46) mess.header.length[i - 44] = buf;
		else if (i < 48) mess.header.checkSum[i - 46] = buf;
		else if (i == 48) mess.body.mavCode = buf;
		else if (i == 49) {
			mess.body.payloadLen = buf;

			payload = (int)buf;
			
			if ((mess.body.payload = (uint8_t *)malloc(sizeof(uint8_t)*payload)) == NULL)
				printf("Memory allocation failed.\n");
		}
		else if (i == 50) mess.body.incompFlag = buf;
		else if (i == 51) mess.body.compFlag = buf;
		else if (i == 52) mess.body.packetSeq = buf;
		else if (i == 53) mess.body.systemID = buf;
		else if (i == 54) mess.body.compID = buf;
		else if (i < 58) mess.body.messageID[i - 55] = buf;		
		else if (i < 58 + payload) mess.body.payload[i - 58] = buf;
		else mess.body.crc[i - 58 - payload] = buf;		
		
		i++;

		// If the end of a message has been reached.
		if (i == (60 + payload) && payload != 0) {
			if (numMess == 0) 
				pcapFile->messages = (message_t *)malloc(sizeof(message_t));
			else 
				pcapFile->messages = (message_t *)realloc(pcapFile->messages, sizeof(message_t)*(numMess + 1));

			pcapFile->messages[numMess] = mess;
					
			// Reset all variables
			i = 0;
			payload = 0;
			numMess++;
		}
	}
	pcapFile->n = numMess;
	
	return pcapFile;
}

/*
 * Free memory from payload, messages array and the structure itself for a pcap file structure.
 * Inputs: pcap file structure.
 * Outputs: none.
 */
void freeMemPcap(pcap_t *pcapFile) {
	// Variable declarations.
	int i;

	for (i = 0; i < pcapFile->n; i++)
		free(pcapFile->messages[i].body.payload);

	free(pcapFile->messages);

	free(pcapFile);
				 
}

/*
 * Free memory from payload, messages array and the structure itself for a messages array structure.
 * Inputs: message array structure.
 * Outputs: none.
 */
void freeMem(messArr_t *messArr) {
	// Variable declarations.
	int i;

	for (i = 0; i < messArr->n; i++)
		free(messArr->messages[i].body.payload);

	free(messArr->messages);

	free(messArr);

}
