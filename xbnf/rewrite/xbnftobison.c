#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RSIZE 2
#define RMIN_IND 0
#define RMAX_IND 1

/* variable defined in xbnf_tb.c */
extern FILE *xout;          /* output bison file */

/* variables used externally in xbnf.y */
int linenum;                /* current line number used for error reporting */
int val;
int num_ranges;              /* index of next range in array */
int rlow;                   /* low value fo range */
int rhigh;                  /* high value of range */

bool ranging;
bool anybyte;

int **ranges;

/* initialise all variables and write bison preamble */
void init(void) {

    /* initialise all variables */
    linenum = 1;
    num_ranges = 0;
    ranging = false;
    anybyte = false;

    /* initialise r__0 for wildcard byte */
    if ((ranges = (int **)malloc(sizeof(int *))) == NULL) {
        printf("failed to allocate memory for ranges\n");
        exit(EXIT_FAILURE);
    }

    if ((ranges[num_ranges] = (int *)malloc(sizeof(int)*RSIZE)) == NULL) {
        printf("failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }

    ranges[num_ranges][RMIN_IND] = 0;
    ranges[num_ranges][RMAX_IND] = 255;

    num_ranges++;

    /* write bison preamble */
    fprintf(xout, "%%{\n");
    fprintf(xout, "\t#define YYDEBUG 1\n");
    fprintf(xout, "\tint yylex(void);\n");
    fprintf(xout, "\tvoid yyerror(char *s);\n");
    fprintf(xout, "%%}\n");
    fprintf(xout, "%%token X00\n");
    fprintf(xout, "%%%%\n");
}

/* write hex values to file */
void hexout(void) {
    if (!ranging) {
        if (val == 0) fprintf(xout, "X00");
        else fprintf(xout, "\'\\x%02x\'", val);
    }
}

/* free all dynamically allocated memory */
void free_mem(void) {
    int i;

    /* free all memory allocated for ranges */
    for (i = 0; i < num_ranges; i++)
        free(ranges[i]);
    free(ranges);
}

/* begins a new range */
void rbegin(void) {
    rlow = -1;
    rhigh = -1;
    val = -1;
    ranging = true;

    if ((ranges = (int **)realloc(ranges, sizeof(int *)*(num_ranges + 1))) == NULL) {
        fprintf(stderr, "failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }

    if ((ranges[num_ranges] = (int *)malloc(sizeof(int)*RSIZE)) == NULL) {
        fprintf(stderr, "failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }
}

void setrlow(void) {
    if (val < 0) {
        fprintf(stderr, "error: line %d - invalid range start\n", linenum);
        exit(EXIT_FAILURE);
    }

    rlow = val;
    val = -1;
}

void setrhigh(void) {
    if (val < 0) {
        fprintf(stderr, "error: line %d - invalid range end\n", linenum);
        exit(EXIT_FAILURE);
    }

    rhigh = val;

    if (rhigh <= rlow) {
        fprintf(stderr, "error: line %d - invalid range [%d-%d] -- low to high required\n", linenum, rlow, rhigh);
        exit(EXIT_FAILURE);
    }

    /* handles case where * was not used */
    if (rlow == 0 && rhigh == 255) {
        fprintf(xout, "r__0");
        anybyte = true;
    }
    else {
        ranges[num_ranges][RMIN_IND] = rlow;
        ranges[num_ranges][RMAX_IND] = rhigh;
        fprintf(xout, "r__%d", num_ranges);
        num_ranges++;
    }

    ranging = false;
}

void addrules(void) {
    int i, j, k;

    /* if a wildcard is present in grammar, start at 0-th index in ranges array */
    if (anybyte) i = 0;
    else i = 1;

    if (anybyte || num_ranges > 1)
        fprintf(xout, "\n/* Range Expansions */\n");

    for (; i < num_ranges; i++) {
        fprintf(xout, "r__%d : ", i);
        for (k = 0, j = ranges[i][RMIN_IND]; j < ranges[i][RMAX_IND]; k++, j++) {
            if (k%8 == 0) fprintf(xout, "\n  ");

            if (j == 0) fprintf(xout, "X00 | ");
            else fprintf(xout, "\'\\x%02x\' | ", (uint8_t)j);
        }

        if (k%8 == 0) fprintf(xout, "\n  ");

        fprintf(xout, "\'\\x%02x\' ;\n\n", (uint8_t)j);
    }
}
