/* 
 * read.c --- 
 * 
 * Author: Joshua M. Meise
 * Created: 06-20-2024
 * Version: 1.0
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct message {
	unsigned char header[50];
	unsigned char *body;
} message_t;

int toInt(unsigned char *len, int numBytes) {
	return (int)len[0]*256 + (int)len[1];
}

void printMessage(message_t mess, int len) {
	int i;

	printf("Message: ");
	for (i = 0; i < len; i++) {
		if (i < 32) printf("%02x ", mess.header[i]);
		else printf("%02x ", mess.body[i - 32]);
	}

	printf("\n");
}

int main(int argc, char **argv) {
	FILE *ifile;
	unsigned char buf;
	int bytesRead, i, len, prevLen;
	message_t mess;
	bool prevMDNS;
	
	if (argc != 2) {                                                                                                                                                                                         
    printf("usage: read fileToRead\n");
    exit(EXIT_FAILURE);
	}

	if ((ifile = fopen(argv[1], "rb")) == NULL) {
    printf("File does not exist\n");
    exit(EXIT_FAILURE);
	}

	i = 0;

	printf("Header: ");
	while(i < 40) {
		bytesRead = fread(&buf, 1, sizeof(buf), ifile);
		printf("%02x ", buf);
		i++;
	}
	printf("\n");
	
	i = 0;
	len = 0;
	prevLen = 0;
	prevMDNS = false;
	
	while ((bytesRead = fread(&buf, 1, sizeof(buf), ifile)) > 0) {

		if ((prevLen == 1366 || prevLen == 148 || prevLen == 1440) && prevMDNS == true) {
			if (i < 52) mess.header[i] = buf;
			else if (i == 52) {
				len = prevLen;
				mess.body = (unsigned char *)malloc(sizeof(unsigned char)*len);
				mess.body[i - 48] = buf;
			} else if (i < len + 52) mess.body[i - 52] = buf;
		} else {
			if (i < 32) mess.header[i] = buf;
			else if (i == 32) {
				len = toInt(&mess.header[28], 2) - 8;
				printf("%d\n", len);
				mess.body = (unsigned char *)malloc(sizeof(unsigned char)*len);
				mess.body[i - 32] = buf;
			} else if (i < len + 32) mess.body[i - 32] = buf;
		}
		
		i++;
		if (((i == (48 + len) && prevMDNS == false) || (i == (68 + len) && prevMDNS == true)) && len != 0) {
			printMessage(mess, len + 32);
			i = 0;
			prevLen = len;
			len = 0;
			free(mess.body);

			if (mess.header[0] == 0x02 && (prevLen == 1366 || prevLen == 148 || prevLen == 1440)) prevMDNS = true;
			else prevMDNS = false;
		}

	}
	
	exit(EXIT_SUCCESS);
}
