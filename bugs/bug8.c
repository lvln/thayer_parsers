/*
 * bug8 -- h_bits(size_t len, bool sign) issues
 * 
 * bug8 is an extension of bug7 with more implementations
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
	 * Parse a single byte, from left to right, consume 2 bits, then 3 bits, a single bit, and then 2 bits.
	 * The values are checked with each set of bits consumed!
	 */
	H_RULE(BITS3, h_bits(3,0));
	H_RULE(BITS1, h_bits(1,0));
	H_RULE(BITS2, h_bits(2,0));
	pp = h_sequence(h_int_range(BITS2,0,0),h_int_range(BITS3,6,6),h_int_range(BITS1,1,1),h_int_range(BITS2,1,1),h_end_p(),NULL);
}

/* Passing Tests: */
static void test_c1() {
	input[0] = 0x35; // 0011 0101
	g_check_parse_ok(pp, BKEND, input, 1);
	/* Result of Parse: 
	 * BITS2:= 0000 0000, BITS3:= 0000 0110, 
	 * BITS1:= 0000 0001, BITS2:= 0000 0001
	 */
}

/* Failing Tests: */
static void test_1val() {
	input[0] = 0x2F; // 0010 1111
	g_check_parse_failed(pp, BKEND, input, 1);
	/* Result of Parse: 
	 * BITS2:= 0000 0000, BITS3:= 0000 0101, 
	 * BITS1:= 0000 0001, BITS2:= 0000 0011
	 */
}

static void test_2val() {
	input[0] = 0x35;
	input[1] = 0x01;
	g_check_parse_failed(pp, BKEND, input, 2);
}


void register_bug_tests() {
	g_test_add_func("/pass/test1",test_c1);
	g_test_add_func("/fail/1val",test_1val);
	g_test_add_func("/fail/2val",test_2val);
}

int main(int argc, char *argv[]) {
	init_parser();
	g_test_init(&argc,&argv,NULL);
	register_bug_tests();
	return g_test_run();
}
