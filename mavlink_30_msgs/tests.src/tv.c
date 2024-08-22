/* 
 * tv.c -- creates test cases for a given message based on its message id
 * 
 * Author: Joshua M. Meise
 * Created: 07-16-2024
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>
#include <inttypes.h>

int main(void) {
	// Variable declarations.
	FILE *ifile, *ofile;
	int i, num;
	char fname[50];
	uint8_t byte;
	
	// HEARTBEAT: change enumerated field
	num = 1;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 14) byte = 0x2d;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// SYS_STATUS: add additional byte
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}
	
	if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
		fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
	fclose(ifile);
	fclose(ofile);

	// SYS_TIME: change incompatibility flag
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 2) byte = 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}
		
	fclose(ifile);
	fclose(ofile);

	// PING: change MAVLink code
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 0) byte = 0xfe;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// LINK_NODE_STATUS: change compatibility flag
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 3) byte = 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// SET_MODE: change payload length field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 2) byte = byte + 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// PARAM_REQUEST_READ: add signature
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	for (i = 0; i < 13; i++)
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);

	fclose(ifile);
	fclose(ofile);

	// PARAM_REQUEST_LIST: write too few bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1 && i < 12) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// PARAM_VALUE: change enumerated field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 34) byte = 0x00;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// PARAM_SET: change enumerated field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 32) byte = 0x00;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// GPS_RAW_INT: change enumerated field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 38) byte = 0x09;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// SCALED_PRESSURE: write too many bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
		fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);

	fclose(ifile);
	fclose(ofile);

	// ATTITUDE:change incompatibility flag
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 2) byte = 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// ATTITUDE_QUATERNION: write too many bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
		fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);

	fclose(ifile);
	fclose(ofile);

	// LOCAL_POSITION_NED: write too few bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1 && i < 20) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// GLOBAL_POSITION_INT: change MAVLink code
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 0) byte = 0xfe;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// MISSION_REQUEST: change enumeerated field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 14) byte = 0xfe;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// MISSION_CURRENT: too many bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
		fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);

	fclose(ifile);
	fclose(ofile);

	// MISSION_REQUEST_LIST: change MAVLink code
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 0) byte = 0xfe;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);
	
	// MISSION_COUNT: change incompatibility flag
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 2) byte = 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// MISSION_CLEAR_ALL: change commpatibility flag
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 3) byte = 0xfe;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// MISSION_ITEM_REACHED: write too many bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
		fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);

	fclose(ifile);
	fclose(ofile);

	// MISSION_ACK: too few bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1 && i < 9) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// GPS_GLOBAL_ORIGIN: change payload length field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 2) byte += 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// MISSION_REQUEST_INT: change MAVLink code
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 0) byte = 0xfe;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// RC_CHANNELS: one extra byte
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
		fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);

	fclose(ifile);
	fclose(ofile);

	// MANUAL_CONTROL: Change incompatibility flag
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 2) byte = 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// MISSION_ITEM_INT: change payload length field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 1) byte += 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// VFR_HUD: change payload length field
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (i == 1) byte += 0x01;
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// COMMAND_LONG: too few bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1 && i < 15) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}

	fclose(ifile);
	fclose(ofile);

	// Full rural flight: write one too many bytes
	num++;
	
	sprintf(fname, "pass.%d", num);
	
	if ((ifile = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}
	
	sprintf(fname, "fail.%d", num);
	
	if ((ofile = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening %s.\n", fname);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (fread(&byte, sizeof(byte), 1, ifile) == 1) {
		if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
			fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
		
		i++;
	}
	
	if (fwrite(&byte, sizeof(byte), 1, ofile) != 1)
		fprintf(stderr, "Problem writing byte %02x to file %s.\n", byte, fname);
	
	fclose(ifile);
	fclose(ofile);

	exit(EXIT_SUCCESS);
}
