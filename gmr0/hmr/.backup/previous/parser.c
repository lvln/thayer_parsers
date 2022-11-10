/*
 * hmr.c -- Hammer parser for JSON
 *
 * Author: Stephen Taylor
 *
 *  * Aug, 31, 2020
 *
 * BNF Grammer:
 * L ::= L ';' E | E
 * E ::= E ',' P | P
 * P ::= 'a' | '(' M ')'
 * M ::= nothing | L
 */
#include "parser.h"

HParser *init_parser() {
	HParser *hmr_parser;

	H_RULE(chara, h_ch('a'));
	H_RULE(obrac, h_ch('('));
	H_RULE(cbrac, h_ch(')'));

	HParser *P = h_indirect();
	HParser *M = h_indirect();

	/* P: 'a' | '(' M ')' */
	h_bind_indirect(P, h_choice(chara, h_sequence(obrac,M,cbrac,NULL), NULL) );

	/* M : empty | P */
	h_bind_indirect(M, h_optional(P) ); 

	/* parser parses a single L followed by end of input */
	hmr_parser = h_sequence(P,h_end_p(),NULL);

	return hmr_parser;
}

