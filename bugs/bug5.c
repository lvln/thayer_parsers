/*
 * bug5 -- h_xor() issues
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
	/* parse a single byte and XOR check if within string "012" or "134" */
	H_RULE(XOR, h_xor(h_in((const uint8_t*)"012", 3), h_in((const uint8_t*)"134", 3)));
	pp = h_sequence(XOR,h_end_p(),NULL);
}

/* Passing Tests: '0', '3' */
static void test_1() {
	input[0] = '0';
	g_check_parse_ok(pp, BKEND, input, 1);
}
static void test_2() {
	input[0] = '3';
	g_check_parse_ok(pp, BKEND, input, 1);
}

/* Failing Tests: '1', '5' */
static void test_3() {
	input[0] = '1';
	g_check_parse_failed(pp, BKEND, input, 1);
}

static void test_4() {
	input[0] = '5';
	g_check_parse_failed(pp, BKEND, input, 1);
}

void register_bug_tests() {
	g_test_add_func("/pass/within first",test_1);
	g_test_add_func("/pass/within second",test_2);
	g_test_add_func("/fail/within both",test_3);
	g_test_add_func("/fail/outside both",test_4);
}

int main(int argc, char *argv[]) {
	init_parser();
	g_test_init(&argc,&argv,NULL);
	register_bug_tests();
	return g_test_run();
}
