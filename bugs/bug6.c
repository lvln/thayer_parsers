/*
 * bug6 -- h_not() issues
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
	/* parse a single int8 between the values of 1 and 15, if h_not succeeds then consume byte */
	H_RULE(NOT, h_sequence(h_not(h_ch_range('\x01','\x0F')),h_uint8(),NULL));
	pp = h_sequence(NOT,h_end_p(),NULL);
}

/* Passing Tests: 0, 165 */
static void test_1() {
	input[0] = 0x00;
	g_check_parse_ok(pp, BKEND, input, 1);
}

static void test_2() {
	input[0] = 0xA5;
	g_check_parse_ok(pp, BKEND, input, 1);
}

/* Failing Tests: +15,+2,0 +1 */
static void test_m2() {
	input[0] = 0x0F; // +15
	g_check_parse_failed(pp, BKEND, input, 1);
}

static void test_p2() {
	input[0] = 0x02; // +2
	g_check_parse_failed(pp, BKEND, input, 1);
}

static void test_2val() {
	input[0] = 0x00; // 0
	input[1] = 0x01; // +1
	g_check_parse_failed(pp, BKEND, input, 2);
}


void register_bug_tests() {
	g_test_add_func("/pass/test1",test_1);
	g_test_add_func("/pass/test2",test_2);
	g_test_add_func("/fail/neg2",test_m2);
	g_test_add_func("/fail/pos2",test_p2);
	g_test_add_func("/fail/2val",test_2val);
}

int main(int argc, char *argv[]) {
	init_parser();
	g_test_init(&argc,&argv,NULL);
	register_bug_tests();
	return g_test_run();
}
