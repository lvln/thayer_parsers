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
	// H_RULE(onebyte,h_sequence(h_int_range(h_uint8(),0,127),NULL));
	// H_RULE(twobyte, h_sequence(h_int_range(h_uint8(),194,223),h_int_range(h_uint8(),128,191),NULL));
	// H_RULE(threebyte, h_sequence(h_int_range(h_uint8(),224,239),h_int_range(h_uint8(),128,191),h_int_range(h_uint8(),128,191),NULL));
	// H_RULE(fourbyte, h_sequence(h_int_range(h_uint8(),240,255),h_int_range(h_uint8(),128,191),h_int_range(h_uint8(),128,191),h_int_range(h_uint8(),128,191),NULL));
	// json_parser = h_choice(onebyte,twobyte,threebyte,fourbyte,h_end_p(),NULL);

	H_RULE(onebyte,h_sequence(h_ch_range(0x00,0x7F),NULL));
	H_RULE(twobyte, h_sequence(h_ch_range(0xC2,0xDF),h_ch_range(0x80,0xBF),NULL));
	H_RULE(threebyte, h_sequence(h_ch_range(0xE0,0xEF),h_ch_range(0x80,0xBF),h_ch_range(0x80,0xBF),NULL));
	H_RULE(fourbyte, h_sequence(h_ch_range(0xF0,0xFF),h_ch_range(0x80,0xBF),h_ch_range(0x80,0xBF),h_ch_range(0x80,0xBF),NULL));
 	json_parser = h_sequence(h_choice(onebyte,twobyte,threebyte,fourbyte,NULL), h_end_p(), NULL);

	return json_parser;
}


