/*
 * hmr.c -- Hammer parser for JSON
 *
 * Author: Stephen Taylor
 *
 */
#include "parser.h"

HParser *init_parser() {
	HParser *json_parser;
	//	HParser *ws = h_indirect(); 
	HParser *jvalue = h_indirect();
	HParser *jmembers = h_indirect();
	HParser *jelements = h_indirect();  

#if 0
	/* whitespace */
	//h_bind_indirect(ws, h_optional(h_sequence(ws,h_in((uint8_t*)" \r\n\t",4),NULL)));
	//H_RULE(ws, h_optional(h_in((uint8_t*)" \r\n\t",4)));
	//	h_bind_indirect(ws, h_optional(h_sequence(ws,h_in((uint8_t*)" \r\n\t",4),NULL)));
	/* literals: these definitions did not work */
	H_RULE(ltrue, h_literal((uint8_t*)"true")); /* literals */
	H_RULE(lfalse, h_literal((uint8_t*)"false"));
	H_RULE(lnull, h_literal((uint8_t*)"null"));
#endif
	H_RULE(ws, h_many(h_in((uint8_t*)" \r\n\t",4)));
	H_RULE(ltrue, h_sequence(h_ch('t'),h_ch('r'),h_ch('u'),h_ch('e'),NULL));
	H_RULE(lfalse, h_sequence(h_ch('f'),h_ch('a'),h_ch('l'),h_ch('s'),h_ch('e'),NULL));
	H_RULE(lnull, h_sequence(h_ch('n'),h_ch('u'),h_ch('l'),h_ch('l'),NULL));
	
	/* numbers */
	H_RULE(dot, h_ch('.'));				
	H_RULE(minus, h_ch('-'));
	H_RULE(plus, h_ch('+'));
	H_RULE(zero, h_ch('0'));
	//	H_RULE(digit, h_ch_range(0x30,0x39));
	H_RULE(onenine, h_ch_range(0x31,0x39));
	H_RULE(digit, h_choice(zero,onenine,NULL));
	H_RULE(exp, h_choice(h_ch('E'), h_ch('e'), NULL));
	H_RULE(jnum, h_sequence(h_optional(minus),
													h_choice(zero,h_sequence(onenine,h_many(digit),NULL), NULL),
													h_optional(h_sequence(dot,h_many1(digit),NULL)),
													h_optional(h_sequence(exp,h_optional(h_choice(plus,minus,NULL)),h_many1(digit),NULL)),
													NULL));

	/* Unicode from bin0 testing */
	H_RULE(twobyte, h_sequence(h_ch_range(0xC2,0xDF),h_ch_range(0x80,0xBF),NULL));
	H_RULE(threebyte, h_sequence(h_ch_range(0xE0,0xEF),h_ch_range(0x80,0xBF),h_ch_range(0x80,0xBF),NULL));
	H_RULE(fourbyte, h_sequence(h_ch_range(0xF0,0xFF),h_ch_range(0x80,0xBF),h_ch_range(0x80,0xBF),h_ch_range(0x80,0xBF),NULL));
	H_RULE(unicodechar, h_sequence(h_choice(twobyte,threebyte,fourbyte,NULL), NULL));
	/* strings */
	H_RULE(dquote, h_ch('"'));
	H_RULE(charb, h_ch('b'));
	H_RULE(charf, h_ch('f'));
	H_RULE(charn, h_ch('n'));
	H_RULE(charr, h_ch('r'));
	H_RULE(chart, h_ch('t'));
	H_RULE(bslash, h_ch('\\'));  		/* backslash 0x5c */
	H_RULE(fslash, h_ch('/'));  		/* fwdslash */
	H_RULE(charu, h_ch('u'));
	H_RULE(uhex, h_ch_range(0x41,0x46));											/* A-F */
	H_RULE(lhex, h_ch_range(0x61,0x66));											/* a-f */
	H_RULE(hex, h_choice(digit,uhex,lhex,NULL));
	H_RULE(uchar, h_choice(uhex,h_ch_range(0x47,0x5A),NULL)); /* and G-Z */
	H_RULE(lchar, h_choice(lhex,h_ch_range(0x67,0x7A),NULL)); /* and g-z */
	H_RULE(unicode, h_sequence(charu,hex,hex,hex,hex,NULL));
	H_RULE(escchar, h_choice(charb,charf,charn,charr,chart,dquote,bslash,fslash,unicode, NULL));
	H_RULE(punct, h_choice(h_ch_range(0x20,0x21),h_ch_range(0x23,0x2F),
												 h_ch_range(0x3A,0x40),h_ch('['),h_ch_range(0x5D,0x60),
												 h_ch_range(0x7b,0x7e),NULL));											/* A-F */
	H_RULE(char1, h_choice(uchar,lchar,digit, unicodechar, punct,h_sequence(bslash,escchar,NULL),NULL));
	H_RULE(chars, h_many(char1));	/* 0 or more chars */
	H_RULE(jstring, h_sequence(dquote,chars,dquote,NULL));
	/* values */

#if 0
	/* numbers */	/* literals */


	H_RULE(jarray, h_middle(lsq, h_sepBy(jelement,comma), rsq));
	H_RULE(jmember, h_sequence(h_middle(ws,jstring,ws), colon, jelement, NULL)); /* string-value pair */
	H_RULE(jobject, h_middle(lcurly, h_optional(h_sepBy(jmember,comma)), rcurly));
	h_bind_indirect(jvalue, h_choice(jobject, jarray, jstring, jnum, ltrue, lfalse, lnull, NULL));
	H_RULE(jmember, h_sequence(ws, jstring, ws, h_ch(':'), jelement, NULL)); /* string-value pair */
	H_RULE(jobject, h_choice(h_sequence(h_ch('{'),ws,h_ch('}'),NULL),
													 h_sequence(h_ch('{'),h_sepBy(jmember,h_ch(',')),h_ch('}'),NULL),
													 NULL));

	
#endif
	/* parser parses a single L followed by end of input */
	H_RULE(lcurl, h_ch('{'));
	H_RULE(rcurl, h_ch('}'));
	H_RULE(lsq, h_ch('['));
	H_RULE(rsq, h_ch(']'));
	H_RULE(colon, h_ch(':'));
	H_RULE(comma, h_ch(','));

	H_RULE(jelement, h_sequence(ws,jvalue,ws,NULL));

	H_RULE(jmember, h_sequence(ws, jstring, ws, colon, jelement, NULL));

	H_RULE(jobject, h_choice(h_sequence(lcurl, ws, rcurl, NULL),
													 h_sequence(lcurl, jmembers, rcurl, NULL),
													 NULL));

	H_RULE(jarray, h_choice(h_sequence(lsq, ws, rsq, NULL),
													h_sequence(lsq, jelements, rsq, NULL),
													 NULL));

	h_bind_indirect(jelements, h_choice(jelement, h_sequence(jelements, comma, jelement, NULL), NULL));
	h_bind_indirect(jmembers, h_choice(jmember, h_sequence(jmembers, comma, jmember, NULL), NULL));
	h_bind_indirect(jvalue, h_choice(jobject, jarray, jstring, jnum, ltrue, lfalse, lnull, NULL));
	json_parser = h_sequence(jelement,h_end_p(),NULL);
	return json_parser;
}

