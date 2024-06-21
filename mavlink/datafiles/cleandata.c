/* 
 * cleandata.c --- cleans a .pcap file such that it only contains MAVLink messages; optional second argument allows selection of a single message
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

// Each message contains a header and a body.
typedef struct message {
	unsigned char header[50];
	unsigned char *body;
} message_t;

// Converts a 32 bit hexadecimal number (stored in an array to an integer.
static int toInt(unsigned char *len, int numBytes) {
	return (int)len[0]*256 + (int)len[1];
}

// For debugging; prints the message to the screen.
static void printMessage(message_t mess, int len) {
	int i;

	printf("Message: ");
	for (i = 0; i < len; i++) {
		if (i < 32) printf("%02x ", mess.header[i]);
		else printf("%02x ", mess.body[i - 32]);
	}

	printf("\n");
}

void writeMessageToFile(message_t mess, int len, FILE *ofile) {
	int i;
	unsigned char buf;
	printf("Length to write = %d\n\n", len);
	for (i = 0; i < len; i++) {
		
		if (i < 48) buf = mess.header[i];
		else buf = mess.body[i - 48];

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Message write failed.\n");
	}
}

int main(int argc, char **argv) {
	FILE *ifile, *ofile;
	unsigned char buf;
	int bytesRead, i, len, prevLen;
	message_t mess;
	bool prevMDNS;
	char *fname;
	
	if (argc != 2) {                                                                                                                                                                                         
    printf("usage: read fileToRead\n");
    exit(EXIT_FAILURE);
	}

	if ((ifile = fopen(argv[1], "rb")) == NULL) {
    printf("File does not exist\n");
    exit(EXIT_FAILURE);
	}

	fname = (char *)malloc(sizeof(char)*(strlen(argv[1]) + 1 + strlen("clean")));

	sprintf(fname, "clean%s", argv[1]);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
    printf("File not opened successfully\n");
    exit(EXIT_FAILURE);
	}

	free(fname);

	i = 0;

	printf("Header: ");
	while(i < 24) {
		bytesRead = fread(&buf, sizeof(buf), 1, ifile);
		printf("%02x ", buf);
		i++;

		if (fwrite(&buf, sizeof(buf), 1, ofile) != 1)
			printf("Not successfully written");
		
	}
	printf("\n");
	
	i = 0;
	len = 0;
	prevLen = 0;
	prevMDNS = false;

	while ((bytesRead = fread(&buf, sizeof(buf), 1, ifile)) > 0) {
		if ((prevLen == 1366 || prevLen == 148 || prevLen == 1440) && prevMDNS == true) {
			if (i < 68) mess.header[i] = buf;
			else if (i == 68) {
				len = prevLen;
				mess.body = (unsigned char *)malloc(sizeof(unsigned char)*len);
				mess.body[i - 68] = buf;
			} else if (i < len + 68) mess.body[i - 68] = buf;
		} else {
			if (i < 48) mess.header[i] = buf;
			else if (i == 48) {
				len = toInt(&mess.header[44], 2) - 8;
				printf("%d\n", len);
				mess.body = (unsigned char *)malloc(sizeof(unsigned char)*len);
				mess.body[i - 48] = buf;
			} else if (i < len + 48) mess.body[i - 48] = buf;
		}
		
		i++;
		if (((i == (48 + len) && prevMDNS == false) || (i == (68 + len) && prevMDNS == true)) && len != 0) {
			printMessage(mess, len + 32);

			if (mess.body[0] == 0xfd)
				writeMessageToFile(mess, len + 48, ofile);
			
			i = 0;
			prevLen = len;
			len = 0;
			free(mess.body);

			if (mess.header[16] == 0x02 && (prevLen == 1366 || prevLen == 148 || prevLen == 1440)) prevMDNS = true;
			else prevMDNS = false;

		}

	}

	fclose(ifile);
	fclose(ofile);
	exit(EXIT_SUCCESS);
}
