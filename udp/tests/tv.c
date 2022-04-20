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

#define MAXDATA 1600

void putbytes(uint16_t val,uint8_t *bp) {
	*bp++ = (uint8_t)(val>>8);
	*bp =  (uint8_t)(0x00&val);
}


int main(int argc, char *argv[]) {

	//first test with 0
	FILE *fp;

	uint8_t num1, num2,num3;

	uint16_t srcport, dstport;
	uint16_t len;
	unit16_t chksum;
	uint16_t data[MAXDATA];
	bool little_endian;

	
	big_endian = true;						/* network byte order */
	srcport = 9000;
	dstport = 1000;
	datalen = 10;
	pktlen = datalen+(4*sizeof(uint16_t));
	chksum = 0;

	num1 = 0;
	num2 = 34;
	num3 = 93;
	fp = fopen("./pass.1", "wb");

	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fwrite(&num2, sizeof(uint8_t), 1, fp);
	fwrite(&num3,sizeof(uint8_t), 1, fp);
	fclose(fp);

	//empty file test
	fp = fopen("./fail.1","wb");
	fclose(fp);

	//only 0 test
	fp = fopen("./fail.2","wb");
	fwrite(&num1, sizeof(uint8_t),1 ,fp);
	fclose(fp);

	//no 34 value in middle
	
	fp = fopen("./fail.3","wb");
	num2 = 45;
  fwrite(&num1, sizeof(uint8_t),1 ,fp);                                 
  fwrite(&num2, sizeof(uint8_t), 1, fp);                                
  fwrite(&num3,sizeof(uint8_t), 1, fp);
	fclose(fp);

	num2 = 34;
	
	//more then the 3 values
	fp = fopen("./fail.4","wb");
	uint8_t num4 = 2;
  fwrite(&num1, sizeof(uint8_t),1 ,fp);                                 
  fwrite(&num2, sizeof(uint8_t), 1, fp);                                
  fwrite(&num3,sizeof(uint8_t), 1, fp);
	fwrite(&num4, sizeof(uint8_t), 1, fp);
                                                                    
  fclose(fp); 

	
 
	return 0;

}
