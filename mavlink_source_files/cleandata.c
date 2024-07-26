/* 
 * cleandata.c --- cleans a .pcap file such that it only contains MAVLink messages; input a pcap file adn output a pcap file
 * 
 * Author: Joshua M. Meise
 * Created: 06-21-2024
 * Version: 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MDNSLEN 68
#define MAVLEN 48

// Each message contains a header and a body.
typedef struct message {
	unsigned char *header;
	unsigned char *body;
} message_t;

/* 
 * Converts a 32 bit hexadecimal number (stored in an array to an integer.
 * Inputs: the start index of the array, how many bytes are in the hex representation of the number
 * Outputs: the integer value of the length of the message
 */
static int toInt(unsigned char *len, int numBytes) {
	return (int)len[0]*256 + (int)len[1];
}

/*
 * Write a message to a file in binary.
 * Inputs: message to be written, length of message (number of bytes), stream to write to.
 * Outputs: none.
 */
void writeMessageToFile(message_t mess, int len, FILE *ofile) {
	int i;
	unsigned char buf;

	for (i = 0; i < len; i++) {		
		if (i < 48) buf = mess.header[i];
		else buf = mess.body[i - 48];

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Message write failed.\n");
	}
}

int main(int argc, char **argv) {
	// Variable declarations
	FILE *ifile, *ofile;
	unsigned char buf;
	int bytesRead, i, len, prevLen;
	message_t mess;
	bool prevMDNS;

	// Check arguments
	if (argc != 3) {
    printf("usage: cleandata inputFile outputFile\n");
    exit(EXIT_FAILURE);
	}
	
	// Check that provided file exists by opening for binary reading
	if ((ifile = fopen(argv[1], "rb")) == NULL) {
    printf("File does not exist\n");
    exit(EXIT_FAILURE);
	}
	
	// Open output file for binary writing
	if ((ofile = fopen(argv[2], "wb")) == NULL) {
    printf("File not opened successfully\n");
		exit(EXIT_FAILURE);
	}
	
	i = 0;

	// Read the first 24 bytes of the .pcap file into the pcap header and write to the output file
	while(i < 24) {
		bytesRead = fread(&buf, sizeof(buf), 1, ifile);
		i++;

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Not successfully written");
		
	}

	printf("\n");
	
	i = 0;
	len = 0;
	prevLen = 0;
	prevMDNS = false;
	
	// Read all of the successive bytes from the file.
	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		// If there are 2 MDNS messages in sequence the length parameter is in a different index.
		if ((prevLen == 1366 || prevLen == 148 || prevLen == 1440) && prevMDNS == true) {
			// Allocate space for header field.
			if (i == 0)
				if ((mess.header = (unsigned char *)malloc(sizeof(unsigned char)*MDNSLEN)) == NULL)
					printf("Memory allocation unsuccessful.\n");
			
			// Read first 68 bytes into header field.
			if (i < MDNSLEN) mess.header[i] = buf;
			// Allocate memory for the message body and assigns the length to the previous length since messages in sequence in this was have the same payload length
			else if (i == MDNSLEN) {
				len = prevLen;

				if ((mess.body = (unsigned char *)malloc(sizeof(unsigned char)*len)) == NULL)
					printf("Memory allocation unsuccessful.\n");

				// Assign the first character of the message body
				mess.body[i - MDNSLEN] = buf;
				
			}
			// The remainder of the characters are inserted into the message body.
			else if (i < len + MDNSLEN) mess.body[i - MDNSLEN] = buf;
		}
		// If reading in a MAVLink message or the first of the sequence of MDNS messages
		else {
			// Allocate space for the header field.
			if (i == 0)
				if ((mess.header = (unsigned char *)malloc(sizeof(unsigned char)*MAVLEN)) == NULL)
					printf("Memory allocation unsuccessful.\n");
	 
			// Readthe first 48 bytes into the header field
			if (i < MAVLEN) mess.header[i] = buf;
			// Find the length of the message body and allocate memory for the body based on the length.
			else if (i == MAVLEN) {
				// Read the hex values in the 44th and 45th cell to determine the length of the message body
				len = toInt(&mess.header[44], 2) - 8;

				// Allocate memory for the message body
				if ((mess.body = (unsigned char *)malloc(sizeof(unsigned char)*len)) == NULL)
					printf("Memory allocation unsuccessful.\n");

				// Assign the first chaacter of the message body
				mess.body[i - MAVLEN] = buf;
			}
			// Insert the remainder of the characters into the message body 
			else if (i < len + MAVLEN) mess.body[i - MAVLEN] = buf;
		}

		i++;

		// If the end of a message has been reached.
		if (((i == (MAVLEN + len) && prevMDNS == false) || (i == (MDNSLEN + len) && prevMDNS == true)) && len != 0) {
			// Only write all  MAVLink messages into the file if 2 arguments
			if (mess.body[0] == 0xfd)
				writeMessageToFile(mess, len + MAVLEN, ofile);

			// Reset all variables
			i = 0;
			prevLen = len;
			len = 0;
			
			// Check the IP protocol (should be 2) and the previous length to determine if the next message to follow is MDNS of varying format
			if (mess.header[16] == 0x02 && (prevLen == 1366 || prevLen == 148 || prevLen == 1440)) prevMDNS = true;
			else prevMDNS = false;

			// Free memory.
			free(mess.header);
			free(mess.body);
		}

	}

	fclose(ifile);
	fclose(ofile);
	exit(EXIT_SUCCESS);
}
