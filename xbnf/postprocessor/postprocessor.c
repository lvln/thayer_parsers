/* 
 * addrule.c --- preprocessor adding rule causing any byte to be skipped over
 * 
 * Author: Joshua M. Meise
 * Created: 08-06-2024
 * Version: 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include <string.h>
#include <stdbool.h>

// Define range structure.
typedef struct range {
	int low;
	int high;
} range_t;

// Define a structure for a state tag.
typedef struct state {
	int number;
	vector_t *tags;
} state_t;

// Define a structure for a transition tag.
typedef struct transition {
	char *type;
	char *symbol;
	int state;
} transition_t;

static char *getTag(FILE *fp, char *tag) {
	// Variable declrations.
	int cnt, i;
	char c;
	
	cnt = 0;
	do {
		fscanf(fp, "%c", &c);
		cnt++;
	} while (c != '>');
	
	if ((tag = (char *)malloc(sizeof(char)*cnt)) == NULL) 
		fprintf(stderr, "Memory allocation failed.\n");
	
	fseek(fp, -1*cnt, SEEK_CUR);
	
	for (i = 0; i < cnt - 1; i++) {
		fscanf(fp, "%c", &c);
		tag[i] = c;
	}
	tag[i] = '\0';

	return tag;
}

static int getNumber(char *str) {
	// Variable declarations.
	int i, j;
	char num[10];

	i = 0;

	while (str[i] != '\0') {
		if (str[i] == '"') {
			i++;
			j = 0;
			do {
				num[j] = str[i];
				i++;
				j++;
			} while (str[i] != '"');
			num[j] = '\0';
		}
		i++;
	}

	return atoi(num);
}

static state_t *getState(FILE *fp, state_t *state) {
	// Variable declarations.
	char *tag, c;
	bool end;
	
	// Allocate memory for new state.
	if ((state = (state_t *)malloc(sizeof(state_t))) == NULL)
		fprintf(stderr, "Memory allocation failed.\n");

	// Open state's vector.
	if ((state->tags = vectorInit()) == NULL) 
		fprintf(stderr, "Memory allocation failed.\n");

	end = false;
	
	// Get all tags and place into vector of tags.
	while (fscanf(fp, "%c", &c) != EOF && end == false) {
		if (c == '<') {
			tag = getTag(fp, tag);

			if (strcmp(tag, "/state") == 0)
				end = true;
			else
				if (vectorInsertBack(state->tags, tag) != 0)
					fprintf(stderr, "Vector insertion unsuccessful.\n");		
		}
	}
	
	free(tag);
	
	return state;
}

static void freeNest(void *data) {
	// Variable declarations.
	state_t *state;

	// Coerce.
	state = (state_t *)data;

	vectorApply(state->tags, free);

	vectorFree(state->tags);
	
	free(state);
}

static bool tagContainsByte(char *data) {
	int i;

	i = 0;
	
	while (data[i] != '\0') {
		if (data[i] == '"') 
			if (data[i - 1] == '=') 
				if (strncmp(data + i + 1, "BYTE", 4) == 0)
					return true;
		i++;
	}

	return false;
}
	

static bool stateContainsByte(vector_t *tags) {
	// Variable declarations.
	int i;
	
	for (i = 0; i < vectorGetSize(tags); i++) {
		if (tagContainsByte((char *)vectorGetElement(tags, i)))
			return true;
	}
	return false;
}

static vector_t *makeRanges(vector_t *byteStates) {
	// Variable declarations.
	int *cur, i, prev;
	vector_t *ranges;
	range_t *range;
	
	// Initialise vector.
	if ((ranges = vectorInit()) == NULL) {
		fprintf(stderr, "Unable to initialise vector.\n");
		return NULL;
	}

	// Loop through, finding consecutive ranges.
	for (i = 0; i < vectorGetSize(byteStates); i++) {
		// Get the value currently stored at the given vector location.
		if ((cur = (int *)vectorGetElement(byteStates, i)) == NULL) {
			fprintf(stderr, "Cannot get element from vector.\n");
			return NULL;
		}

		// Create a new range and set the low bound on the range.
		if (i == 0) {
			if ((range = (range_t *)malloc(sizeof(range_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return NULL;
			}

			range->low = *cur;

			// Add range into vector.
			if (vectorInsertBack(ranges, (void *)range) != 0)
				fprintf(stderr, "Problem inserting range into vector.\n");

		}
		// If reaching the initial value of a new range.
		else if (*cur != prev + 1) {
			// Set the high on the previous range.
			range->high = prev;
			
			// Begin a new range.
			if ((range = (range_t *)malloc(sizeof(range_t))) == NULL) {
				fprintf(stderr, "Memory allocation failed.\n");
				return NULL;
			}

			range->low = *cur;
			
			// Add range into vector.
			if (vectorInsertBack(ranges, (void *)range) != 0)
				fprintf(stderr, "Problem inserting range into vector.\n");
		}
		
		prev = *cur;
	}

	// Set the high on the last range.
	range->high = prev;

	return ranges;
	
}

int main(int argc, char **argv) {
	// Variable declarations.
	FILE *fp, *ofile;;
	vector_t *stateVec, *byteStates, *ranges;
	char *tag, c, fileStr[500], newRule[2000000], string[100];
	state_t *state;
	range_t *range;
	int i;
	bool byte, done;
	
	// Check arguments.
	if (argc != 3) {
		printf("usage: %s inputXMLFile.xml inputCFile.c\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Open file.
	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Unable to open file.\n");
		exit(EXIT_FAILURE);
	}

	// Initialise the vector of states.
	if ((stateVec = vectorInit()) == NULL)
		fprintf(stderr, "Unable to initialise vector.\n");

	// Initialise the vector of states.
	if ((byteStates = vectorInit()) == NULL)
		fprintf(stderr, "Unable to initialise vector.\n");	

	while (fscanf(fp, "%c", &c) != EOF) {
		if (c == '<') {
			tag = getTag(fp, tag);
			
			//			printf("%s\n", tag);
			
			if (strncmp(tag, "state", 5) == 0) {
				state = getState(fp, state);
				state->number = getNumber(tag);
				if (vectorInsertBack(stateVec, state) != 0)
					fprintf(stderr, "Vector insertion unsuccessful.\n");
				
			}
			
			free(tag);
			
		}

	}
	fclose(fp);

	for (i = 0; i < vectorGetSize(stateVec); i++) {
		state = (state_t *)vectorGetElement(stateVec, i);
		byte = stateContainsByte(state->tags);

		if (byte)
			if (vectorInsertBack(byteStates, (void *)&(state->number)) != 0)
				fprintf(stderr, "Vector insertion unsuccessful.\n");
	}

	if (vectorGetSize(byteStates) > 0) {
		if ((ranges = makeRanges(byteStates)) == NULL)
			fprintf(stderr, "Unable to build range.\n");

		strcpy(newRule, "      if (");
		for (i = 0; i < vectorGetSize(ranges) - 1; i++) {
			// Get range from vector.
			if ((range = (range_t *)vectorGetElement(ranges, i)) == NULL)
				fprintf(stderr, "Could not get range from vector.\n");

			// Create rule depending on whether or not it is a range.
			if (range->low == range->high) sprintf(string, "yystate == %d || ", range->low);
			else sprintf(string, "(yystate >= %d && yystate <= %d) || ", range->low, range->high);
			
			strcpy(newRule + strlen(newRule), string);
		}

		// Get range from vector.
		if ((range = (range_t *)vectorGetElement(ranges, i)) == NULL)
			fprintf(stderr, "Could not get range from vector.\n");
		
		// Create rule depending on whether or not it is a range.
		if (range->low == range->high) sprintf(string, "yystate == %d) yychar = BYTE;\n", range->low);
		else sprintf(string, "(yystate >= %d && yystate <= %d)) yychar = BYTE;\n", range->low, range->high);

		strcpy(newRule + strlen(newRule), string);

		vectorApply(ranges, free);
		vectorFree(ranges);
	}

	if ((fp = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "File not opened successfully.\n");
		exit(EXIT_FAILURE);
	}

	if ((ofile = fopen("tmp.c", "w")) == NULL) {
		fprintf(stderr, "File not opened successfully.\n");
		exit(EXIT_FAILURE);
	}

	done = false;
	
	while (fgets(fileStr, 500, fp) != NULL) {
		if (strncmp(fileStr + strlen(fileStr) - 32, "yytoken = YYTRANSLATE (yychar);\n", 32) == 0 && done == false) {
			fprintf(ofile, "%s", newRule);
			done = true;
		}
		fprintf(ofile, "%s", fileStr);
	}

	vectorFree(byteStates);
	vectorApply(stateVec, freeNest);
	vectorFree(stateVec);

	fclose(fp);
	fclose(ofile);

	fp = fopen("tmp.c", "r");
	ofile = fopen("gmr.tab.c", "w");

	while (fscanf(fp, "%c", &c) != EOF)
		fprintf(ofile, "%c", c);

	fclose(fp);
	fclose(ofile);

	remove("tmp.c");
	
	exit(EXIT_SUCCESS);
}
