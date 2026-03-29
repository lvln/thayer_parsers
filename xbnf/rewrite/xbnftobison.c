#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "types.h"

/* variable defined in xbnf_tb.c */
extern FILE* xout;          /* output bison file */

/* variables used externally in xbnf.y */
int line_num;                /* current line number used for error reporting */
val_t val;

int num_ranges;              /* index of next range in array */
val_t r_low;                   /* low value fo range */
val_t r_high;                  /* high value of range */
range_t* ranges;

int num_enums;
enum_t* enums;

char** strings;
int num_strings;
int num_chars;

bool in_string;
bool ranging;
bool any_byte;

static char get_esc_char(char c) {
    switch (c) {
        case '\b':
            return 'b';
            break;
        case '\f':
            return 'f';
            break;
        case '\n':
            return 'n';
            break;
        case '\r':
            return 'r';
            break;
        case '\t':
            return 't';
            break;
        case '\\':
            return '\\';
            break;
        case '\"':
            return '\"';
            break;
        case (char)27:
            return (char)27;
            break;
        case '\v':
            return 'v';
            break;
        case '\?':
            return '?';
            break;
        case '\a':
            return 'a';
            break;
        case '\'':
            return '\'';
            break;
        default:
            fprintf(stderr, "unknown escaped character on line %d\n", line_num);
            exit(EXIT_FAILURE);
    }
}

/* initialise all variables and write bison preamble */
void init(void) {

    /* initialise all variables */
    line_num = 1;
    num_ranges = 0;
    in_string = false;
    ranging = false;
    any_byte = false;

    /* initialise r__0 for wildcard byte */
    if ((ranges = (range_t*)malloc(sizeof(range_t))) == NULL) {
        fprintf(stderr, "failed to allocate memory for ranges\n");
        exit(EXIT_FAILURE);
    }

    ranges[num_ranges].min.val.uint8 = 0;
    ranges[num_ranges].max.val.uint8 = 255;
    ranges[num_ranges].min.t = uint8;
    ranges[num_ranges].max.t = uint8;
    num_ranges++;

    enums = NULL;
    num_enums = 0;

    strings = NULL;
    num_strings = 0;

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
void hex_out(void) {
    if (!ranging) {
        if (val.val.uint8 == 0) fprintf(xout, "X00");
        else fprintf(xout, "\'\\x%02x\'", val.val.uint8);
    }
}

void char_out(void) {
    if (!ranging && !in_string) {
        if ((val.val.uint8 >= 7 && val.val.uint8 <= 13) || val.val.uint8 == 27 || val.val.uint8 == '\'' || val.val.uint8 == '\"' || val.val.uint8 == '\\')
            fprintf(xout, "\'\\%c\'", (char)val.val.uint8);
        else fprintf(xout, "\'%c\'", (char)val.val.uint8);
    }
}

char get_esc_val(char c) {
    switch (c) {
        case 'b':
            return '\b';
            break;
        case 'f':
            return '\f';
            break;
        case 'n':
            return '\n';
            break;
        case 'r':
            return '\r';
            break;
        case 't':
            return '\t';
            break;
        case '\\':
            return '\\';
            break;
        case '\"':
            return '\"';
            break;
        case 'e':
            return (char)27;
            break;
        case 'v':
            return '\v';
            break;
        case '\?':
            return '\?';
            break;
        case 'a':
            return '\a';
            break;
        case '\'':
            return '\'';
            break;
        default:
            fprintf(stderr, "unknown escaped character on line %d\n", line_num);
            exit(EXIT_FAILURE);
    }
}

/* free all dynamically allocated memory */
void free_mem(void) {
    int i;

    free(ranges);

    for (i = 0; i < num_enums; i++)
        free(enums[i].vals);
    free(enums);

    for (i = 0; i < num_strings; i++)
        free(strings[i]);
    free(strings);
}

void set_r_low(void) {
    if (val.val.uint8 < 0) {
        fprintf(stderr, "error: line %d - invalid range start\n", line_num);
        exit(EXIT_FAILURE);
    }

    if ((ranges = (range_t*)realloc(ranges, sizeof(range_t)*(num_ranges + 1))) == NULL) {
        fprintf(stderr, "failed to allocate memory for range\n");
        exit(EXIT_FAILURE);
    }

    // TODO: Add in cses here for different datatypes.
    r_low.val.uint8 = val.val.uint8;
    r_low.t = uint8;
}

void set_r_high(void) {
    if (val.val.uint8 < 0) {
        fprintf(stderr, "error: line %d - invalid range end\n", line_num);
        exit(EXIT_FAILURE);
    }

    // TODO: Add in cases here for different datatypes.
    r_high.val.uint8 = val.val.uint8;
    r_high.t = uint8;

    if (r_high.val.uint8 <= r_low.val.uint8) {
        fprintf(stderr, "error: line %d - invalid range [%d-%d] -- low to high required\n", line_num, r_low.val.uint8, r_high.val.uint8);
        exit(EXIT_FAILURE);
    }

    /* handles case where * was not used */
    if (r_low.val.uint8 == 0 && r_high.val.uint8 == 255) {
        fprintf(xout, "r__0");
        any_byte = true;
    }
    else {
        ranges[num_ranges].min.val.uint8 = r_low.val.uint8;
        ranges[num_ranges].max.val.uint8 = r_high.val.uint8;
        ranges[num_ranges].min.t = uint8;
        ranges[num_ranges].max.t = uint8;
        fprintf(xout, "r__%d", num_ranges);
        num_ranges++;
    }

    ranging = false;
}

void e_start(void) {
    /* adding an additional enumeration */
    if (enums == NULL) {
        if ((enums = (enum_t*)malloc(sizeof(enum_t))) == NULL) {
            fprintf(stderr, "failed to allocate memory for enums\n");
            exit(EXIT_FAILURE);
        }
    }
    else if ((enums = (enum_t*)realloc(enums, sizeof(enum_t)*(num_enums + 1))) == NULL) {
        fprintf(stderr, "failed to allocate memory for enums\n");
        exit(EXIT_FAILURE);
    }

    /* add first value to enumeration */
    if ((enums[num_enums].vals = (val_t*)malloc(sizeof(val_t))) == NULL) {
        fprintf(stderr, "failed to allocate memory for enum\n");
        exit(EXIT_FAILURE);
    }

    enums[num_enums].num_vals = 1;
    enums[num_enums].vals[0].val.uint8 = val.val.uint8;
    enums[num_enums].vals[0].t = uint8;
}

void set_next_enum(void) {
    /* grow array of values */
    if ((enums[num_enums].vals = (val_t*)realloc(enums[num_enums].vals, sizeof(val_t)*(enums[num_enums].num_vals + 1))) == NULL) {
        fprintf(stderr, "failed to allocate memory for enum\n");
        exit(EXIT_FAILURE);
    }

    /* add value to enumerationa array */
    enums[num_enums].vals[enums[num_enums].num_vals].val.uint8 = val.val.uint8;
    enums[num_enums].vals[enums[num_enums].num_vals].t = uint8;
    enums[num_enums].num_vals++;
}

void e_end(void) {
    if (enums[num_enums].num_vals == 1) {
        fprintf(stderr, "error: line %d - single entry enum prohibited\n", line_num);
        exit(EXIT_FAILURE);
    }

    num_enums++;
    ranging = false;

    fprintf(xout, "e__%d", num_enums);
}

void s_begin(void) {
    in_string = true;

    if (strings == NULL) {
        if ((strings = (char**)malloc(sizeof(char*))) == NULL) {
            fprintf(stderr, "failed to allocate memory for strings\n");
            exit(EXIT_FAILURE);
        }
    } else if ((strings = (char**)realloc(strings, sizeof(char*)*(num_strings + 1))) == NULL) {
        fprintf(stderr, "failed to allocate memory for strings\n");
        exit(EXIT_FAILURE);
    }

    /* allocate memory for first character */
    if ((strings[num_strings] = (char*)malloc(sizeof(char))) == NULL) {
        fprintf(stderr, "failed to allocate memory for string\n");
        exit(EXIT_FAILURE);
    }

    strings[num_strings][0] = val.val.uint8;
    num_chars = 1;
}

void s_add(void) {
    /* increare size of string */
    if ((strings[num_strings] = (char*)realloc(strings[num_strings], sizeof(char)*(num_chars + 1))) == NULL) {
        fprintf(stderr, "failed to allocate memory for string\n");
        exit(EXIT_FAILURE);
    }

    strings[num_strings][num_chars] = val.val.uint8;
    num_chars++;
}

void s_end(void) {

    /* increase size of string */
    if ((strings[num_strings] = (char*)realloc(strings[num_strings], sizeof(char)*(num_chars + 1))) == NULL) {
        fprintf(stderr, "failed to allocate memory for string\n");
        exit(EXIT_FAILURE);
    }

    strings[num_strings][num_chars] = '\0';
    num_strings++;

    fprintf(xout, "s__%d", num_strings);

    in_string = false;
}

void add_rules(void) {
    int i, j, k, cval;

    /* if a wildcard is present in grammar, start at 0-th index in ranges array */
    if (any_byte) i = 0;
    else i = 1;

    if (any_byte || num_ranges > 1)
        fprintf(xout, "\n/* Range Expansions */\n");

    for (; i < num_ranges; i++) {
        fprintf(xout, "r__%d : ", i);
        for (k = 0, j = ranges[i].min.val.uint8; j < ranges[i].max.val.uint8; k++, j++) {
            if (k%8 == 0) fprintf(xout, "\n  ");

            if (j == 0) fprintf(xout, "X00 | ");
            else fprintf(xout, "\'\\x%02x\' | ", (uint8_t)j);
        }

        if (k%8 == 0) fprintf(xout, "\n  ");
        fprintf(xout, "\'\\x%02x\' ;\n\n", (uint8_t)j);
    }

    if (num_enums > 0) {
        fprintf(xout, "\n/* Enumerations */\n");

        for (i = 0; i < num_enums; i++) {
            fprintf(xout, "e__%d : ", i);
            for (j = 0; j < enums[i].num_vals - 1; j++) {
                if (j%8 == 0) fprintf(xout, "\n  ");

                if (enums[i].vals[j].val.uint8 == 0) fprintf(xout, "X00 | ");
                else fprintf(xout, "\'\\x%02x\' | ", (uint8_t)enums[i].vals[j].val.uint8);
            }

            if (j%8 == 0) fprintf(xout, "\n  ");
            if (enums[i].vals[j].val.uint8 == 0) fprintf(xout, "X00 ;\n\n");
            else fprintf(xout, "\'\\x%02x\' ;\n\n", (uint8_t)enums[i].vals[j].val.uint8);
        }
    }

    /* NOTE: Escaped characters in string will not print backslash as a literal. */
    if (num_strings > 0) {
        fprintf(xout, "\n/* Strings */\n");

        for (i = 0; i < num_strings; i++) {
            fprintf(xout, "s__%d :\n  ", i);
            j = 0;
            while ((cval = (int)strings[i][j]) != 0) {
                if ((cval >= 7 && cval <= 13) || cval == 27 || cval == '\'' || cval == '\"' || cval == '\\')
                    fprintf(xout, "\'\\%c\' ", get_esc_char((char)cval));
                else fprintf(xout, "\'%c\' ", (char)cval);
                j++;
            }
            fprintf(xout, ";\n\n");
        }
    }
}
