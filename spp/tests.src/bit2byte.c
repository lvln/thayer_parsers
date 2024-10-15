/* 
 * bit2byte.c --- spp header conversion
 * 
 * Author: Stephen Taylor
 * Created: 10-08-2024
 * Version: 1.0
 * 
 * Description: bit to byte conversion routine for parsing spp packets
 * 
 */
#include <bit2byte.h>

void bit2byteHeader(uint8_t bits[6], uint8_t bytes[10]) {
	/* expand the header */
	bytes[0] = bits[0] >> 5;			/* 3 bits */
	bytes[1] = (bits[0] & 0x10) >> 4; /* 1 bit */
	bytes[2] = (bits[0] & 0x08) >> 3; /* 1 bit */
	bytes[3] = (bits[0] & 0x07);			/* 3 bits */
	bytes[4] = bits[1];						/* 8 bits */
	bytes[5] = bits[2] >> 6;			/* 2 bits */
	bytes[6] = bits[2] & 0x3F;		/* 6 bits */
	bytes[7] = bits[3];						/* 8 bits */
	bytes[8] = bits[4];						/* 8 bits */
	bytes[9] = bits[5];						/* 8 bits */
}

/* 
 * bit2byte -- convert an spp packet into a byte based header followed
 * by its payload.
 */
void bit2bytePacket(uint8_t bits[],int len, uint8_t bytes[],int *newlen) {
	int i;

	/* expand the header */
	bit2byteHeader(bits,bytes);
	/* copy the payload */
	for(i=0; i<len-6; i++)				/* rest of data without 6 byte header */
		bytes[10+i]=bits[6+i];			/* added into the new 10 byte header */
	*newlen = len+4;							/* 6 byte header become 10 bytes; 4 bytes added */
}


#ifdef DEBUG

#define MAXBUF 1024

static void printbytes(char *msg,uint8_t bytes[],int len) {
	int i;
	printf("%s",msg);
	for(i=0; i<len; i++) {
		if(i%16==10)
			printf("\n");
		printf("%02" PRIx8 " ",bytes[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	FILE *fp;
	uint8_t input[MAXBUF],output[MAXBUF],byte;
	int cnt,newcnt;
	
	if(argc!=3 && argc!=4) {
		printf("usage: bit2byte <bitfile> <bytefile> [-p]\n");
		printf("   -p : convert entire packet (default just header)\n");
		exit(EXIT_FAILURE);
	}

	/* read the input file */
	if((fp=fopen(argv[1],"r"))==NULL) {
		printf("unable to open %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	cnt=0;
	while(fread(&byte,sizeof(uint8_t),1,fp)==1) 
		input[cnt++]=byte;
	fclose(fp);

	/* do the conversion */
	printbytes("in:     ",input,cnt);
	if(argc==4 && strcmp(argv[3],"-p")==0) {
		cnt=6;											/* convert the header */
		newcnt=10;
		bit2byteHeader(input,output);
	}
	else													/* convert the entire packet */
		bit2bytePacket(input,cnt,output,&newcnt);
	printbytes("out:    ",output,newcnt);

	/* generate the output file */
	if((fp=fopen(argv[2],"w"))==NULL) {
		printf("unable to open %s\n",argv[2]);
		exit(EXIT_FAILURE);
	}
	cnt=0;
	while(newcnt>0) {
		byte=output[cnt++];
		if(fwrite(&byte,sizeof(uint8_t),1,fp)!=1) {
			printf("unable to write %s\n",argv[2]);
			exit(EXIT_FAILURE);
		}
		newcnt--;
	}
	fclose(fp);
	exit(EXIT_SUCCESS);
}

#endif	/* DEBUG */

