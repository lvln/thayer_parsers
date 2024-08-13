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

// Define public data structures.
typedef void pcap_t;
typedef void mavlink_t;
typedef void tlog_t;

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
 * Print out the entire contents of a stored PCAP file.
 * Inputs: PCAP file
 * Outputs: none
 */
void printFilePcap(pcap_t *pcapP);

/*
 * Print out the entire contents of a stored MAVlink file.
 * Inputs: MAVLink file
 * Outputs: none
 */
void printFileMav(mavlink_t *mavP);

/*
 * Print out the entire contents of a stored TLOG file.
 * Inputs: TLOG file
 * Outputs: none
 */
void printFileTlog(tlog_t *tlogP);

/*
 * Write the entire contents of a PCAP file structure to a file.
 * Inputs: PCAP file, file to write to
 * Outputs: none
 */
void writeToFilePcap(pcap_t *pcapP, FILE *fp);

/*
 * Write the entire contents of a MAVLink file structure to a file.
 * Inputs: MAVLink file, file to write to
 * Outputs: none
 */
void writeToFileMav(mavlink_t *mavP, FILE *fp);

/*
 * Write the entire contents of a TLOG file structure to a file.
 * Inputs: TLOG file, file to write to
 * Outputs: none
 */
void writeToFileTlog(tlog_t *tlogP, FILE *fp);

/*
 * Frees a PCAP file structure and all messages associated with it.
 * Inputs: PCAP file
 * Outputs: none
 */
void freePcap(pcap_t *pcapP);

/*
 * Frees a MAVLink file structure and all messages associated with it.
 * Inputs: MAVLink file
 * Outputs: none
 */
void freeMav(mavlink_t *mavP);

/*
 * Frees a TLOG file structure and all messages associated with it.
 * Inputs: TLOG file
 * Outputs: none
 */
void freeTlog(tlog_t *tlogP);

/*
 * Convert a TLOG file structure to a MAVLink file structure.
 * Inputs: TLOG file structure
 * Outputs: MAVLink file structure; NUKLL if unsuccessful
 */
mavlink_t *tlogToMav(tlog_t *tlogP);

/*
 * Convert a TLOG file structure to a PCAP file structure.
 * Inputs: TLOG file structure
 * Outputs: PCAP file structure; NUKLL if unsuccessful
 */
pcap_t *tlogToPcap(tlog_t *tlogP);

/*
 * Convert a PCAP file structure to a MAVLink file structure.
 * Inputs: PCAP file structure
 * Outputs: MAVLink file structure; NUKLL if unsuccessful
 */
mavlink_t *pcapToMav(pcap_t *pcapP);

/*
 * Count the number of unique message IDs in a PCAP file.
 * Inputs: PCAP file structure
 * Outputs: none
 */
void countIDSPcap(pcap_t *pcapP);

/*
 * Count the number of unique message IDs in a MAVLink file.
 * Inputs: MAVLink file structure
 * Outputs: none
 */
void countIDSMav(mavlink_t *mavP);

/*
 * Count the number of unique message IDs in a TLOG file.
 * Inputs: TLOG file structure
 * Outputs: none
 */
void countIDSTlog(tlog_t *tlogP);

/*
 * Extract all messages of a given ID(s) from a PCAP file and place in a MAVLink file.
 * Inputs: PCAP file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByIdPcapToMav(pcap_t *pcapP, vector_t *ids);

/*
 * Extract all messages of a given ID(s) from a TLOG file and place in a MAVLink file.
 * Inputs: TLOG file, vector of IDs
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByIdTlogToMav(tlog_t *tlogP, vector_t *ids);

/*
 * Extract messages of a given number in the message sequence (indexed base 1).
 * Inputs: PCAP file, vector of message numbers
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByNumberPcapToMav(pcap_t *pcapP, vector_t *numbers);

/*
 * Extract messages of a given number in the message sequence (indexed base 1).
 * Inputs: TLOG file, vector of message numbers
 * Outputs: MAVLink file; NULL if unsuccessful
 */
mavlink_t *extractByNumberTlogToMav(tlog_t *tlogP, vector_t *numbers);

/* Generate a full suite of tests for a given messageID.
 * Inputs: message ID, initial pass test number, initial fail test number
 * Outputs: none
 */
void generateTests(int messageID, int passSeed, int failSeed);
