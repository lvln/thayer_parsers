/*
 * parser.c -- Hammer parser for JSON numbers
 *
 * Author: Stephen Taylor
 *
 *  * Aug, 31, 2020
 *
 * Revision by Ben Cavanagh Mar 7, 2026:
 * Reformat to increase readability for presentation, match xBNF
 *
 */
#include "parser.h"

HParser *init_parser() {
    HParser *jnum_parser;

    /* Terminal chars */
    H_RULE(sign, h_choice(h_ch('+'), h_ch('-'), NULL));
    H_RULE(onenine, h_ch_range(0x31,0x39));
    H_RULE(digit, h_choice(h_ch('0'), onenine, NULL));

    H_RULE(digits, h_many1(digit));

    H_RULE(integer, h_sequence(h_optional(h_ch('-')), h_choice(digit, h_sequence(onenine, digits, NULL), NULL), NULL));
    H_RULE(fraction, h_sequence(h_ch('.'), digits, NULL));
    H_RULE(exponent, h_sequence(h_choice(h_ch('E'), h_ch('e'), NULL), h_optional(sign), digits, NULL));

    H_RULE(jnum, h_sequence(integer, h_optional(fraction), h_optional(exponent), NULL));
    jnum_parser = h_sequence(jnum, h_end_p(), NULL);

    return(jnum_parser);
}

