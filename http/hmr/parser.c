/*
 * hmr.c -- Hammer parser for JSON numbers
 *
 * Author: Stephen Taylor
 *
 *  * Aug, 31, 2020
 *
 */
#include "parser.h"

HParser *init_parser() {
	HParser *json_parser;

	/* numbers */
	H_RULE(dot, h_ch('.'));				
	H_RULE(minus, h_ch('-'));
	H_RULE(plus, h_ch('+'));
	H_RULE(zero, h_ch('0'));
	H_RULE(digit, h_ch_range(0x30,0x39));
	H_RULE(onenine, h_ch_range(0x31,0x39));
	H_RULE(exp, h_choice(h_ch('E'), h_ch('e'), NULL));
	H_RULE(jnum, h_sequence(h_optional(minus),
													h_choice(zero,h_sequence(onenine,h_many(digit),NULL), NULL),
													h_optional(h_sequence(dot,h_many1(digit),NULL)),
													h_optional(h_sequence(exp,h_optional(h_choice(plus,minus,NULL)),h_many1(digit),NULL)),
													NULL));

	json_parser = h_sequence(jnum,h_end_p(),NULL);

	return json_parser;
}

