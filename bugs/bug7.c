/*
 * bug7 -- h_bits(size_t len, bool sign) issues
 *
 * This parser passes all tests if the backend is PB_MIN but fails if
 * the backend is PB_LALR.
 *
 * See defines for BKEND below.
 */
#include <hammer/hammer.h>
#include <hammer/glue.h>
#include <hammer/test_suite.h>
#include <glib.h>

#ifndef BKEND
#define BKEND PB_LALR
#endif

const HParser *pp; /* the parser */

uint8_t input[1024]; /* the input is a buff of bytes */

/* The Parser */
void init_parser() {
	/*
	 * Parse a single byte, from left to right, consume 3 bits then 5 bits.
	 * Does NOT check for value, only consumes bits!
	 */
	H_RULE(BITS3, h_bits(3,0));
	H_RULE(BITS5, h_bits(5,0));
	pp = h_sequence(BITS3,BITS5,h_end_p(),NULL);
}

/* Passing Tests: */
static void test_c1() {
	input[0] = 0x3F; // 0011 1111
	g_check_parse_ok(pp, BKEND, input, 1);
	/* Result: 
	 * BITS3:= 0000 0001, BITS5 := 0001 1111
	 */
}

/* Failing Tests: */
static void test_2val() {
	input[0] = 0x01;
	input[1] = 0x01;
	g_check_parse_failed(pp, BKEND, input, 2);
}


void register_bug_tests() {
	g_test_add_func("/pass/char1",test_c1);
	g_test_add_func("/fail/2val",test_2val);
}

int main(int argc, char *argv[]) {
	init_parser();
	g_test_init(&argc,&argv,NULL);
	register_bug_tests();
	return g_test_run();
}
