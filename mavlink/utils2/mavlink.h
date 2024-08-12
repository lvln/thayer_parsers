#pragma once
/* 
 * mavlink.h --- header file for fuunctions to operate on mavlink data
 * 
 * Author: Joshua M. Meise
 * Created: 06-27-2024
 * Version: 1.0
 * 
 * Description: 
 * 
 */

// Library inclusions.
#include <stdint.h>
#include <stdio.h>
#include <vector.h>

// Packet record header
typedef struct pcaketRecordHeader {
	uint8_t timeS[4];
	uint8_t timeUNS[4];
	uint8_t capturedPacketLength[4];
	uint8_t originalPacketLength[4];
} packetRecordHeader_t;

// Ethernet header.
typedef struct ethernetHeader {
	uint8_t messageFam[4];
	uint8_t iphl;
	uint8_t dsf;
	uint8_t totalLength[2];
	uint8_t id[2];
	uint8_t ffOffset[2];
	uint8_t ttl;
	uint8_t protocol;
	uint8_t headerChecksum[2];
	uint8_t sourceAddr[4];
	uint8_t destAddr[4];
	uint8_t sourcePort[2];
	uint8_t destPort[2];
	uint8_t length[2];
	uint8_t checkSum[2];
} ethernetHeader_t;

// TLOG header.
typedef struct tlogHeader {
	uint8_t bytes[8];
} tlogHeader_t;

// MAVLink 1 message.
typedef struct mav1Message {
	uint8_t mavCode;
	uint8_t payloadLen;
	uint8_t packetSeq;
	uint8_t systemID;
	uint8_t compID;
	uint8_t messageID;
	uint8_t *payload;
	uint8_t crc[2];
} mav1Message_t;

// MAVLink 2 message.
typedef struct mav2Message {
	uint8_t mavCode;
	uint8_t payloadLen;
	uint8_t incompFlag;
	uint8_t compFlag;
	uint8_t packetSeq;
	uint8_t systemID;
	uint8_t compID;
	uint8_t messageID[3];
	uint8_t *payload;
	uint8_t crc[2];
	bool signedMess;
	uint8_t signature[13];
} mav2Message_t;

// MAVLink message is either MAVLink 1 or MAVLink 2.
typedef union mavMessage {
	mav1Message_t mav1;
	mav2Message_t mav2;
} mavMessage_t;

// A TLOG message contains a tlog header and a MAVLink message.
typedef struct tlogMessage {
	tlogHeader_t header;
	mavMessage_t mav;
} tlogMessage_t;

// A pcap message contains a packet record header, an ethernet header and a MAVLink message.
typedef struct pcapMessage {
	packetRecordHeader_t prh;
	ethernetHeader_t eth;
	mavMessage_t mav;
} pcapMessage_t;

// Data structure to hold a TLOG file.
typedef struct tlog {
	vector_t *messages;
} tlog_t;

// Data structure to hold a MAVLink file.
typedef struct mavlink {
	vector_t *messages;
} mavlink_t;

// Data structure to hold a pcap file.
typedef struct pcap {
	uint8_t header[24];
	vector_t *messages;
} pcap_t;

/*
 * Read a PCAP file (header and all messages).
 * Inputs: input file
 * Outputs: PCAP file data structure; NULL if unsuccessful
 */
pcap_t *readFilePcap(FILE *ifile);

/*
 * Read a MAVlink file (all messages).
 * Inputs: input file
 * Outputs: MAVLink file data structure; NULL if unsuccessful
 */
mavlink_t *readFileMav(FILE *ifile);

/*
 * Read a TLOG file (all messages).
 * Inputs: input file
 * Outputs: TLOG file data structure; NULL if unsuccessful
 */
tlog_t *readFileTlog(FILE *ifile);

/*
 * Print out a message including its PCAP wrappers.
 * Inputs: message to print
 * Outputs: none
 */
void printMessagePcap(pcapMessage_t *mess);

/*
 * Print out a MAVLink message.
 * Inputs: message to print
 * Outputs: none
 */
void printMessageMav(mavMessage_t *mess);

/*
 * Print out a TLOG message.
 * Inputs: message to print
 * Outputs: none
 */
void printMessageTlog(tlogMessage_t *mess);

/*
 * Print out the entire contents of a stored PCAP file.
 * Inputs: PCAP file
 * Outputs: none
 */
void printFilePcap(pcap_t *pcap);

/*
 * Print out the entire contents of a stored MAVlink file.
 * Inputs: MAVLink file
 * Outputs: none
 */
void printFileMav(mavlink_t *mav);

/*
 * Print out the entire contents of a stored TLOG file.
 * Inputs: TLOG file
 * Outputs: none
 */
void printFileTlog(tlog_t *tlog);

/*
 * Write a PCAP message to a file.
 * Inputs: message to write, file to write to
 * Outputs: none
 */
void writeMessageToFilePcap(pcapMessage_t *mess, FILE *fp);

/*
 * Write a MAVLink message to a file.
 * Inputs: message to write, file to write to
 * Outputs: none
 */
void writeMessageToFileMav(mavMessage_t *mess, FILE *fp);

/*
 * Write a TLOG message to a file.
 * Inputs: message to write, file to write to
 * Outputs: none
 */
void writeMessageToFileTlog(tlogMessage_t *mess, FILE *fp);

/*
 * Write the entire contents of a PCAP file structure to a file.
 * Inputs: PCAP file, file to write to
 * Outputs: none
 */
void writeToFilePcap(pcap_t *pcap, FILE *fp);

/*
 * Write the entire contents of a MAVLink file structure to a file.
 * Inputs: MAVLink file, file to write to
 * Outputs: none
 */
void writeToFileMav(mavlink_t *mav, FILE *fp);

/*
 * Write the entire contents of a TLOG file structure to a file.
 * Inputs: TLOG file, file to write to
 * Outputs: none
 */
void writeToFileTlog(tlog_t *tlog, FILE *fp);

/*
 * Frees a PCAP file structure and all messages associated with it.
 * Inputs: PCAP file
 * Outputs: none
 */
void freePcap(pcap_t *pcap);

/*
 * Frees a MAVLink file structure and all messages associated with it.
 * Inputs: MAVLink file
 * Outputs: none
 */
void freeMav(mavlink_t *mav);

/*
 * Frees a TLOG file structure and all messages associated with it.
 * Inputs: TLOG file
 * Outputs: none
 */
void freeTlog(tlog_t *tlog);

/*
 * Convert a TLOG file structure to a MAVLink file structure.
 * Inputs: TLOG file structure
 * Outputs: MAVLink file structure; NUKLL if unsuccessful
 */
mavlink_t *tlogToMav(tlog_t *tlog);

/*
 * Convert a TLOG file structure to a PCAP file structure.
 * Inputs: TLOG file structure
 * Outputs: PCAP file structure; NUKLL if unsuccessful
 */
pcap_t *tlogToPcap(tlog_t *tlog);

/*
 * Convert a PCAP file structure to a MAVLink file structure.
 * Inputs: PCAP file structure
 * Outputs: MAVLink file structure; NUKLL if unsuccessful
 */
mavlink_t *pcapToMav(pcap_t *pcap);

/*
 * Convert a TLOG message to a MAVLink message.
 * Inputs: TLOG message
 * Outputs: MAVLink message; NULL if unsuccessful
 */
mavMessage_t *tlogMessToMav(tlogMessage_t *tMess);

/*
 * Convert a TLOG message to a PCAP message.
 * Inputs: TLOG message
 * Outputs: PCAP message; NULL if unsuccessful
 */
pcapMessage_t *tlogMessToPcap(tlogMessage_t *tMess);

/*
 * Convert a PCAP message to a MAVLink message.
 * Inputs: PCAP message
 * Outputs: MAVLink message; NULL if unsuccessful
 */
mavMessage_t *pcapMessToMav(pcapMessage_t *pMess);

/*
 * Count the number of unique message IDs in a PCAP file.
 * Inputs: PCAP file structure
 * Outputs: none
 */
void countIDSPcap(pcap_t *pcap);

/*
 * Count the number of unique message IDs in a MAVLink file.
 * Inputs: MAVLink file structure
 * Outputs: none
 */
void countIDSMav(mavlink_t *mav);

/*
 * Count the number of unique message IDs in a TLOG file.
 * Inputs: TLOG file structure
 * Outputs: none
 */
void countIDSTlog(tlog_t *tlog);

/*
 * Extract all messages of a given ID(s) from a PCAP file and place in a MAVLink file.
 * Inputs: PCAP file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByIdPcapToMav(pcap_t *pcap, vector_t *ids);
