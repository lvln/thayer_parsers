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

	H_RULE(ANYBYTE, h_uint8());

	json_parser = h_sequence(ANYBYTE,h_end_p(),NULL);

	return json_parser;
}

