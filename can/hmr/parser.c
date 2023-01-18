/*
 * parser.c -- Hammer parser for CAN
 *
 * Author: Joshua Renfro
 *
 */
#include "parser.h"

HParser *init_parser() {
	/*    GENERAL RULES    */
	H_RULE(CONST_0__U1, h_int_range(h_bits(1, false), 0x0, 0x0));
	H_RULE(ANY_VALUE__U11, h_bits(11, false));
	H_RULE(CONST_0__U2, h_int_range(h_bits(2, false), 0x0, 0x0));
	H_RULE(DLC, h_int_range(h_put_value(h_bits(4, false), "dlc"), 0, 8));
	H_RULE(ANY_VALUE__U8_ARRAY_DLC, h_length_value(h_get_value("dlc"), h_uint8()));
	H_RULE(ANY_VALUE__U15, h_bits(15, false));
	H_RULE(CONST_1__U1, h_int_range(h_bits(1, false), 0x1, 0x1));
	H_RULE(CONST_127__U7, h_int_range(h_bits(7, false), 0x7f, 0x7f));


	/*    MAIN RULES    */
	H_RULE(CAN_MESSAGE, h_sequence(CONST_0__U1, ANY_VALUE__U11, CONST_0__U1, CONST_0__U2, DLC, ANY_VALUE__U8_ARRAY_DLC, ANY_VALUE__U15, CONST_1__U1, CONST_1__U1, CONST_1__U1, CONST_127__U7, NULL));


	/*    FINAL RULE    */
	H_RULE(PARSER, h_choice(CAN_MESSAGE, NULL));

	return PARSER;
}

