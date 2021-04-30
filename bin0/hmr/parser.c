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
	H_RULE(bin0, h_int_range(h_uint8(), 0,0));
	H_RULE(bin34, h_int_range(h_uint8(), 34,34));
	H_RULE(bin93, h_int_range(h_uint8(), 93, 93));
	json_parser = h_sequence(bin0, bin34, bin93,h_end_p(),NULL);
	return json_parser;
}

