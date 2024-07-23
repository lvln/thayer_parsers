/* 
 * tv.c -- extracts a single message from a file containing data from drone flight
 *         writes each message to a file individually
 *         iterates over all non-constant data fields and changes values generating passing tests
 *         iterates over all constant fieldsa dn changes values generating failing tests
 * 
 * Author: Joshua M. Meise
 * Created: 07-16-2024
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <mavlink.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Structure which contains a message id and a boolean value to indicate whether or not tests have been generated for said id.
typedef struct codeDone {
	int code;
	bool done;
} codeDone_t;

// Striucture that contains an array of message ids and the number of elements in said array.
typedef struct array {
	codeDone_t *codeDone;
	int n;
} array_t;

// Structure that holds an array of integers and number of elements in array.
typedef struct arrSize {
	int *arr;
	int n;
} arrSize_t;

/*
 * Writes a specified number of bytes of a MAVLink message to a file.
 * Inputs: message to write, pointer to file to write, number of bytes to write, payload length of the message
 * Outputs: none
 */
static void writeDiffLenFile(messageBody_t mess, FILE *fp, int len, int payload) {
	// Variable declarations.
	int i;
	uint8_t buf;

	i = 0;

	// Write specified number of bytes to file.
	while (i < len) {
		if (i == 0) buf = mess.mavCode;
		else if (i == 1) buf =	mess.payloadLen;
		else if (i == 2) buf = mess.incompFlag;
		else if (i == 3) buf = mess.compFlag;
		else if (i == 4) buf = mess.packetSeq;
		else if (i == 5) buf = mess.systemID;
		else if (i == 6) buf = mess.compID;
		else if (i < 10) buf = mess.messageID[i - 7];		
		else if (i < 10 + payload) buf = mess.payload[i - 10];
		else if (i < 12 + payload) buf = mess.crc[i - 10 - payload];
		else buf = 0x21;
		
		i++;

		if (fwrite(&buf, sizeof(buf), 1, fp) != 1)
			printf("Error writing byte %02x to file.\n", buf);
	}
}

/*
 * See if an id is already present an an array of ids.
 * Inputs: array of ids, id to search for
 * Outputs: true if present; false otherwise
 */
static bool contains(array_t *arr, int comp) {
	// Varaible declarations.
	int i;

	for (i = 0; i < arr->n; i++) {
		if (arr->codeDone[i].code == comp)
			return true;
	}

	return false;
}

/*
 * Counts the number of unique message ids in a file and places those codes in an array.
 * Inputs: Array of messages
 * Outputs: Array of codes
 */
static array_t *countCodes(mavMessArr_t *messArr) {
	// Variable declarations.
	int i;
	array_t *arr;

	// Allocate memory for an arrya of message ids.
	if ((arr = (array_t *)malloc(sizeof(array_t))) == NULL)
		printf("Problem with memory allocation.\n");

	// Initialise the array of ids.
	arr->codeDone = NULL;
	arr->n = 0;
	
	for (i = 0; i < messArr->n; i++) {
		// If the first code being inserted then allocate an array for it an insert the code. Also increment the number of elements in the array.
		if (arr->n == 0) {
			arr->codeDone = (codeDone_t *)malloc(sizeof(codeDone_t));
			arr->codeDone[arr->n].code = toInt24le(messArr->messages[i].messageID);
			arr->codeDone[arr->n].done = false;
			(arr->n)++;
		}
		// If the code is not contained in the array, add it into the array by reallocating space.
		else if (!(contains(arr,toInt24le(messArr->messages[i].messageID)))) {
			arr->codeDone = (codeDone_t *)realloc(arr->codeDone, sizeof(codeDone_t)*(arr->n + 1));
			arr->codeDone[arr->n].code = toInt24le(messArr->messages[i].messageID);
			arr->codeDone[arr->n].done = false;
			(arr->n)++;
		}
	}
				
	return arr;		
}

/*
 * Frees memory dynalically alloced for array of message ids.
 * Inputs: array of message ids
 * Outputs: none
 */
static void freeArr(array_t *arr) {
	// Free the embedded array.
	free(arr->codeDone);

	// Free the structure itself.
	free(arr);
}

/*
 * Checks if a given value is found within an array.
 * Inputs: array of values, value to find
 * Outputs: true if found, false otherwise
 */
static bool arrContains(arrSize_t arr, int val) {
	// Variable declarations.
	int i;

	for (i = 0; i < arr.n; i++) {
		if (arr.arr[i] == val)
			return true;
	}

	return false;
}

/*
 * Returns a value that is within the enumerated range for a message.
 * Inputs: message ID, filed index
 * Outputs: value in range
 */
static uint8_t fillEnumInRange(int msgID, int ind) {
	// Variable declarations.
	uint8_t val;

	// Except in a few cases, the byte 0x01 will suffice.
	if (msgID == POSITION_TARGET_GLOBAL_INT && ind == 50)
		val = 0x00;
	else
		val = 0x01;

	return val;
}

/*
 * Writes the actual tests to files based on valid ennumerated values.
 * Inputs: message to write to file, field index, valid enumerated values, maximum length of message, message ID
 * Outputs: none
 */
static void gt(messageBody_t mess, int ind, arrSize_t arr, int maxLen, int msgID) {
	// Variable declarations.
	int i;
	char fname[50];
	FILE *fp;
	
	for (i = 0; i <= 255; i++) {
		mess.payload[ind] = (uint8_t)i;
		
		// If a valid enumrated value, make it a pass test.
		if (arrContains(arr, i)) {
			// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
			sprintf(fname, "./pass.%d.%d.%d.%d", msgID, maxLen, ind, i);
			
			// Open file for writing.
			if ((fp = fopen(fname, "wb")) == NULL) {
				printf("Error opening file %s for writing.\n", fname);
				exit(EXIT_FAILURE);
			}
			
			// Write message to file.
			writeMavMessageToFile(mess, fp);
			
			fclose(fp);
		}
		// Else make it a failure test.
		else {
			// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
			sprintf(fname, "./fail.%d.%d.%d.%d", msgID, maxLen, ind, i);
			
			// Open file for writing.
			if ((fp = fopen(fname, "wb")) == NULL) {
				printf("Error opening file %s for writing.\n", fname);
				exit(EXIT_FAILURE);
			}
			
			// Write message to file.
			writeMavMessageToFile(mess, fp);
			
			fclose(fp);
		}
	}
}

/*
 * Generates passing and failing tests for enumerated fields
 * Inputs: message ID, field index, message
 * Outputs: none
 */
static void generateEnumTests(int msgID, int ind, messageBody_t mess) {
  // Variable declarations.
	arrSize_t arr;
	int i;

	// Fill in the valid enumerated values for each message type and field.
	switch(msgID) {
	case HEARTBEAT:
		if (ind == 4) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*45)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 45;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, HEARTBEAT_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 5) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*21)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 21;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, HEARTBEAT_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 7) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*9)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 9;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, HEARTBEAT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case PARAM_VALUE:
		if (ind == 24) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*10)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 10;

			// Fill array.
			for (i = 1; i <= arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, PARAM_VALUE_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case GPS_RAW_INT:
		if (ind == 28) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*9)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 9;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, GPS_RAW_INT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case MISSION_CURRENT:
		if (ind == 4) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*6)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 6;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, MISSION_CURRENT_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 5) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 3;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, MISSION_CURRENT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case COMMAND_ACK:
		if (ind == 2) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*10)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 10;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, COMMAND_ACK_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case POSITION_TARGET_LOCAL_NED:
		if (ind == 50) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*4)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 4;

			// Fill array.
			arr.arr[0] = 0x01;
			arr.arr[1] = 0x07;
			arr.arr[2] = 0x08;
			arr.arr[3] = 0x09;

			// Generate the test files.
			gt(mess, ind, arr, POSITION_TARGET_LOCAL_NED_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case POSITION_TARGET_GLOBAL_INT:
		if (ind == 50) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*6)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 6;

			// Fill array.
			arr.arr[0] = 0x01;
			arr.arr[1] = 0x03;
			arr.arr[2] = 0x0A;
			arr.arr[3] = 0x05;
			arr.arr[4] = 0x06;
			arr.arr[5] = 0x0B;

			// Generate the test files.
			gt(mess, ind, arr, POSITION_TARGET_GLOBAL_INT_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case BATTERY_STATUS:
		if (ind == 33) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 34) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 40) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*8)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 8;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 49) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 3;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, BATTERY_STATUS_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case EXTENDED_SYS_STATE:
		if (ind == 0) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, EXTENDED_SYS_STATE_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 1) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, EXTENDED_SYS_STATE_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case STATUSTEXT:
		if (ind == 0) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*8)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 8;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, STATUSTEXT, msgID);
			
			free(arr.arr);
		}
		break;
	case UTM_GLOBAL_POSITION:
		if (ind == 68) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			arr.arr[0] = 0x01;
			arr.arr[1] = 0x02;
			arr.arr[2] = 0x03;
			arr.arr[3] = 0x10;
			arr.arr[4] = 0x20;

			// Generate the test files.
			gt(mess, ind, arr, UTM_GLOBAL_POSITION_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case OPEN_DRONE_ID_LOCATION:
		if (ind == 52) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 53) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 2;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 54) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*13)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 13;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 55) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*7)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 7;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 56) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*7)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 7;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 57) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*5)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 5;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 58) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*15)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 15;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_LOCATION_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	case OPEN_DRONE_ID_SYSTEM:
		if (ind == 50) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 3;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_SYSTEM_LEN, msgID);
			
			free(arr.arr);
		}
		else if (ind == 51) {
			// Allocate memory for array.
			if ((arr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
				printf("Problem with memory allocation.\n");
		 	
			arr.n = 2;

			// Fill array.
			for (i = 0; i < arr.n; i++)
				arr.arr[i] = i;

			// Generate the test files.
			gt(mess, ind, arr, OPEN_DRONE_ID_SYSTEM_LEN, msgID);
			
			free(arr.arr);
		}
		break;
	default:
		return;
	}
	
}

/*
 * Generats test cases for a given message id.
 * Inputs: message id
 * Outputs: none
 */
static void generateTests(int msgID) {
	// Variable declarations.
	messageBody_t mess;
	int i, j, maxLen;
	uint8_t randomByte;
	char fname[50];
	FILE *fp;
	arrSize_t enumArr;

	// Fill in which fields are enumerated for each message type.
	switch(msgID) {
	case HEARTBEAT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*3)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 4;
		enumArr.arr[1] = 5;
		enumArr.arr[2] = 7;

		enumArr.n = 3;

		maxLen = HEARTBEAT_LEN;
		break;
	case SYS_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = SYS_STATUS_LEN;
		break;
	case PING:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = PING_LEN;
		break;
	case LINK_NODE_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = LINK_NODE_STATUS_LEN;
		break;
	case PARAM_VALUE:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 24;

		enumArr.n = 1;

		maxLen = PARAM_VALUE_LEN;
		break;
	case GPS_RAW_INT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 28;

		enumArr.n = 1;

		maxLen = GPS_RAW_INT_LEN;
		break;
	case SCALED_PRESSURE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = SCALED_PRESSURE_LEN;
		break;
	case ATTITUDE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ATTITUDE_LEN;
		break;
	case ATTITUDE_QUATERNION:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ATTITUDE_QUATERNION_LEN;
		break;
	case LOCAL_POSITION_NED:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = LOCAL_POSITION_NED_LEN;
		break;
	case GLOBAL_POSITION_INT:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = GLOBAL_POSITION_INT_LEN;
		break;
	case MISSION_CURRENT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 4;
		enumArr.arr[1] = 5;

		enumArr.n = 2;

		maxLen = MISSION_CURRENT_LEN;
		break;
	case MISSION_ITEM_REACHED:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = MISSION_ITEM_REACHED_LEN;
		break;
	case RC_CHANNELS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = RC_CHANNELS_LEN;;
		break;
	case VFR_HUD:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = VFR_HUD_LEN;
		break;
	case COMMAND_ACK:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 2;

		enumArr.n = 1;

		maxLen = COMMAND_ACK_LEN;
		break;
	case ATTITUDE_TARGET:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ATTITUDE_TARGET_LEN;
		break;
	case POSITION_TARGET_LOCAL_NED:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 50;

		enumArr.n = 1;

		maxLen = POSITION_TARGET_LOCAL_NED_LEN;
		break;
	case POSITION_TARGET_GLOBAL_INT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 50;

		enumArr.n = 1;

		maxLen = POSITION_TARGET_GLOBAL_INT_LEN;
		break;
	case RADIO_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = RADIO_STATUS_LEN;
		break;
	case ALTITUDE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ALTITUDE_LEN;
		break;
	case BATTERY_STATUS:
		if((enumArr.arr = (int *)malloc(sizeof(int)*4)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 33;
		enumArr.arr[1] = 34;
		enumArr.arr[2] = 40;
		enumArr.arr[3] = 49;

		enumArr.n = 4;

		maxLen = BATTERY_STATUS_LEN;
		break;
	case ESTIMATOR_STATUS:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = ESTIMATOR_STATUS_LEN;
		break;
	case VIBRATION:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = VIBRATION_LEN;
		break;
	case HOME_POSITION:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = HOME_POSITION_LEN;
		break;
	case EXTENDED_SYS_STATE:
		if((enumArr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 0;
		enumArr.arr[1] = 1;

		enumArr.n = 2;

		maxLen = EXTENDED_SYS_STATE_LEN;
		break;
	case STATUSTEXT:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 0;

		enumArr.n = 1;

		maxLen = STATUSTEXT_LEN;
		break;
	case UTM_GLOBAL_POSITION:
		if((enumArr.arr = (int *)malloc(sizeof(int)*1)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 68;

		enumArr.n = 1;

		maxLen = UTM_GLOBAL_POSITION_LEN;
		break;
	case TIME_ESTIMATE_TO_TARGET:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = TIME_ESTIMATE_TO_TARGET_LEN;
		break;
	case EVENT:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = EVENT_LEN;
		break;
	case CURRENT_EVENT_SEQUENCE:
		enumArr.arr = NULL;
		enumArr.n = 0;

		maxLen = CURRENT_EVENT_SEQUENCE_LEN;
		break;
	case OPEN_DRONE_ID_LOCATION:
		if((enumArr.arr = (int *)malloc(sizeof(int)*7)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 52;
		enumArr.arr[1] = 53;
		enumArr.arr[2] = 54;
		enumArr.arr[3] = 55;
		enumArr.arr[4] = 56;
		enumArr.arr[5] = 57;
		enumArr.arr[6] = 58;

		enumArr.n = 7;

		maxLen = OPEN_DRONE_ID_LOCATION_LEN;
		break;
	case OPEN_DRONE_ID_SYSTEM:
		if((enumArr.arr = (int *)malloc(sizeof(int)*2)) == NULL)
			printf("Error with memory allocation.\n");

		enumArr.arr[0] = 50;
		enumArr.arr[1] = 51;

		enumArr.n = 2;

		maxLen = OPEN_DRONE_ID_SYSTEM_LEN;
		break;
	default:
		return;
	}

	// Fill header fields for MAVLink 2 tests.
	mess.mavCode = 0xfd;
	mess.incompFlag = 0x00;
	mess.compFlag = 0x00;
	fromInt24le(msgID, mess.messageID);
	
	// Generate a passing test case for each payload length.
	for (i = 1; i <= maxLen; i++) {
		// Set wildcard fields to have random byte values.
		mess.packetSeq = (uint8_t)(rand() % 256);
		mess.systemID = (uint8_t)(rand() % 256);
		mess.compID = (uint8_t)(rand() % 256);
		
		// Set the payload length field.
		mess.payloadLen = (uint8_t)i;
		
		// Allocate memory for payload.
		mess.payload = (uint8_t *)malloc(sizeof(uint8_t) * i);
		
		// Place random bytes into payload (except on ennumerated fields).
		for (j = 0; j < i; j++) {
			// Generate a random byte for wildcard fields.
			randomByte = (uint8_t)(rand() % 256);

			// Differentiate between ennumerated and non-ennumerated fields.
			if (!(arrContains(enumArr, j))) mess.payload[j] = randomByte;
			else mess.payload[j] = fillEnumInRange(msgID, j);
		}

		// Fill in the crc with random bytes.
		mess.crc[0] = (uint8_t)(rand() % 256);
		mess.crc[1] = (uint8_t)(rand() % 256);

		// Create file with name pass.msgID.len where len is the length of the payload being tested.
		sprintf(fname, "./pass.%d.%d", msgID, i);

		// Open file for writing.
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("Error opening file %s for writing.\n", fname);
			exit(EXIT_FAILURE);
		}

		// Write message to file.
		writeMavMessageToFile(mess, fp);
		
		fclose(fp);

		// Free memory from payload if not full length.
		if (i != maxLen)
			free(mess.payload);
	}

	// Now test a full length message with all fields tested for various bytes.
	for (i = 0; i < maxLen; i++) {		
		// Set wildcard fields to have random byte values.
		mess.packetSeq = (uint8_t)(rand() % 256);
		mess.systemID = (uint8_t)(rand() % 256);
		mess.compID = (uint8_t)(rand() % 256);

		// Deal with non-ennumerated fields.
		if (!(arrContains(enumArr, i))) {
			// Generate tests for each byte value for the given field.
			for (j = 0; j <= 255; j++) {
				mess.payload[i] = (uint8_t)j;

				// Create file with name pass.msgID.maxLen.field.val where field is the number of the payload field and val is the byte value being tested.
				sprintf(fname, "./pass.%d.%d.%d.%d", msgID, maxLen, i, j);
				
				// Open file for writing.
				if ((fp = fopen(fname, "wb")) == NULL) {
					printf("Error opening file %s for writing.\n", fname);
					exit(EXIT_FAILURE);
				}
				
				// Write message to file.
				writeMavMessageToFile(mess, fp);
				
				fclose(fp);
			}

			// Fill the field with a random value.
			mess.payload[i] = (uint8_t)(rand() % 266);
		}
		// Deal with enumerated fields producing passing and failing tests.
		else {
			// Generate passing and failing tests for enumerated fields.
			generateEnumTests(msgID, i, mess);

			// Reset the payload of the field to be valid.
			mess.payload[i] = fillEnumInRange(msgID, i);
		}

		// Generate failing tests for each message type (not on enumerated fields).
		
	}
	
	free(mess.payload);

	if (enumArr.arr != NULL)
		free(enumArr.arr);
}

int main(int argc, char **argv) {
	// Variable declarations.
	int id;
	
	// Seed random number generator.
	srand(time(NULL));

	if (argc != 2) {
		printf("usage: tv msgID\n");
		exit(EXIT_FAILURE);
	}

	id = atoi(argv[1]);
	
	generateTests(id);

	exit(EXIT_SUCCESS);
}
