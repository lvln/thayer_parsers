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
	/* terminal symbols */
	H_RULE(chara, h_ch('a'));
	H_RULE(obrac, h_ch('('));
	H_RULE(cbrac, h_ch(')'));
	H_RULE(comma, h_ch(','));
	H_RULE(semic, h_ch(';'));
	/* forward declarations */
	HParser *L = h_indirect();  
	HParser *E = h_indirect();  
	HParser *P = h_indirect();
	HParser *M = h_indirect();
	/* L: L ',' E | E */
	h_bind_indirect(L, h_choice(h_sequence(L,semic,E,NULL), E, NULL));
	/* E: E ',' P | P */
	h_bind_indirect(E, h_choice(h_sequence(E,comma,P,NULL), P, NULL));
	/* P: 'a' | '(' M ')' */
	h_bind_indirect(P, h_choice(chara, h_sequence(obrac,M,cbrac,NULL), NULL) );
	/* M : empty | L */
	h_bind_indirect(M, h_optional(L) ); 
	/* parser parses a single L followed by end of input */
	hmr_parser = h_sequence(L,h_end_p(),NULL);
	return hmr_parser;
}

