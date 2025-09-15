#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RSIZE 2
#define RMIN 0
#define RMAX 1

/* variable defined in xbnf_tb.c */
extern FILE *xout;          /* output bison file */

/* variables used externally in xbnf.y */
int linenum;                /* current line number used for error reporting */
int hval;                   /* hexadecimal value of ascii character */
int cval;                   /* holds charracer value */
int nextrange;
int rlow;
int rhigh;

bool ranging;
bool anybyte;

int **ranges;

/* initialise all variables and write bison preamble */
void init(void) {

    /* initialise all variables */
    linenum = 1;
    nextrange = 0;
    ranging = false;
    anybyte = false;

    /* initialise r__0 for wildcard byte */
    if ((ranges = (int **)malloc(sizeof(int *))) == NULL) {
        printf("failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }

    if ((ranges[nextrange] = (int *)malloc(sizeof(int)*RSIZE)) == NULL) {
        printf("failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }

    ranges[nextrange][RMIN] = 0;
    ranges[nextrange][RMAX] = 255;

    nextrange++;

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
        if (hval == 0) fprintf(xout, "X00");
        else fprintf(xout, "\'\\x%02x\'", hval);
    }
}

void free_mem(void) {
    int i;

    /* free all memory allocated for ranges */
    for (i = 0; i < nextrange; i++)
        free(ranges[i]);

    free(ranges);
}

/* begins a new range */
void rbegin(void) {
    rlow = -1;
    rhigh = -1;
    cval = -1;
    hval = -1;
    ranging = true;

    if ((ranges = (int **)realloc(ranges, sizeof(int *)*(nextrange + 1))) == NULL) {
        printf("failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }

    if ((ranges[nextrange] = (int *)malloc(sizeof(int)*RSIZE)) == NULL) {
        printf("failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }
}

void setrlow(void) {
    if (hval < 0 && cval < 0) {
        printf("error: line %d - invalid range start\n", linenum);
        exit(EXIT_FAILURE);
    }

    if (hval >= 0) rlow=hval;
    else rlow = cval;


    hval = -1;
    cval = -1;
}

void setrhigh(void) {
    if (hval < 0 && cval < 0) {
        printf("error: line %d - invalid range end\n", linenum);
        exit(EXIT_FAILURE);
    }

    if (hval >= 0) rhigh = hval;
    else rhigh = cval;

    if (rhigh <= rlow) {
        printf("error: line %d - invalid range [%d-%d] -- low to high required\n", linenum, rlow, rhigh);
        exit(EXIT_FAILURE);
    }

    if (rlow == 0 && rhigh == 255) {
        fprintf(xout, "r__0");
        anybyte = true;
    }
    else {
        ranges[nextrange][RMIN] = rlow;
        ranges[nextrange][RMAX] = rhigh;
        fprintf(xout, "r__%d", nextrange);
        nextrange++;
    }

    ranging = false;
}

void addrules(void) {
    int i, j, k;

    /* if a wildcard is present in grammar, start at 0-th index in ranges array */
    if (anybyte) i = 0;
    else i = 1;

    if (anybyte || nextrange > 1)
        fprintf(xout, "\n/* Range Expansions */\n");

    for (; i < nextrange; i++) {
        fprintf(xout, "r__%d : ", i);
        for (k = 0, j = ranges[i][RMIN]; j < ranges[i][RMAX]; k++, j++) {
            if (k%8 == 0) fprintf(xout, "\n  ");

            if (j == 0) fprintf(xout, "X00 | ");
            else fprintf(xout, "\'\\x%02x\' | ", (uint8_t)j);
        }

        if (k%8 == 0) fprintf(xout, "\n  ");

        fprintf(xout, "\'\\x%02x\' ;\n", (uint8_t)j);
    }
}
