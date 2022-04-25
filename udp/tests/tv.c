/* 
 * tv.c --- generates a set of UDP packets
 * 
 * Author: Stephen Taylor
 * Created: Apr 19, 22
 * Version: 1.0
 * 
 * Description: Generates both good and bad udp messages.  Good
 * packets are predefined. Bad packets have an illegal checksum, or
 * length.
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(int argc, char *argv[]) {
	FILE *fp;

	uint16_t srcport, dstport;
	uint16_t datalen, pktlen;
	uint16_t chksum;
	int i;
	uint8_t byte;
	
	srcport = 9000;
	dstport = 1000;
	datalen = 10;
	pktlen = (4*sizeof(uint16_t)) + datalen;
	chksum = 0;

	fp = fopen("./pass.1", "wb");
	fwrite(&srcport,sizeof(uint16_t),1 ,fp);
	fwrite(&dstport,sizeof(uint16_t), 1, fp);
	fwrite(&pktlen,sizeof(uint16_t), 1, fp);
	fwrite(&chksum,sizeof(uint16_t), 1, fp);
	for(i=0; i<datalen; i++) {
		byte = (uint8_t)(i%256);
		fwrite(&byte,sizeof(uint8_t), 1, fp);
	}
	fclose(fp);
	return 0;
}
