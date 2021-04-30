/*
 * hmr.c -- Hammer parser for JSON strings
 *
 * Author: Stephen Taylor
 *
 *  * Aug, 31, 2020
 */
#include "parser.h"

HParser *init_parser() {
	HParser *json_parser;

	/* strings */
	H_RULE(quote, h_ch('"'));
	H_RULE(charb, h_ch('b'));
	H_RULE(charf, h_ch('f'));
	H_RULE(charn, h_ch('n'));
	H_RULE(charr, h_ch('r'));
	H_RULE(chart, h_ch('t'));
	H_RULE(bslash, h_ch('\\'));  		/* backslash 0x5c */
	H_RULE(fslash, h_ch('/'));  		/* fwdslash */
	H_RULE(charu, h_ch('u'));
	H_RULE(digit, h_ch_range(0x30,0x39));											/* 0-9 */
	H_RULE(uhex, h_ch_range(0x41,0x46));											/* A-F */
	H_RULE(lhex, h_ch_range(0x61,0x66));											/* a-f */
	H_RULE(hex, h_choice(digit,uhex,lhex,NULL));
	H_RULE(uchar, h_choice(uhex,h_ch_range(0x47,0x5A),NULL)); /* and G-Z */
	H_RULE(lchar, h_choice(lhex,h_ch_range(0x67,0x7A),NULL)); /* and g-z */
	H_RULE(unicode, h_sequence(charu,hex,hex,hex,hex,NULL));
	H_RULE(escchar, h_choice(charb,charf,charn,charr,chart,quote,bslash,fslash,unicode,NULL));
	H_RULE(punct, h_choice(h_ch_range(0x20,0x21),h_ch_range(0x23,0x2F),
												 h_ch_range(0x3A,0x40),h_ch('['),h_ch_range(0x5D,0x60),
												 h_ch_range(0x7b,0x7e),NULL));											/* A-F */
	H_RULE(char1, h_choice(uchar,lchar,digit,punct,h_sequence(bslash,escchar,NULL),NULL));
	H_RULE(chars, h_many(char1));	/* 0 or more chars */
	H_RULE(jstring, h_sequence(quote,chars,quote,NULL));
	/* parser parses a single L followed by end of input */
	json_parser = h_sequence(jstring,h_end_p(),NULL);

	return json_parser;
}

