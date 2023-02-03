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
	/* parse a string to differentiate between two strings "a++b" and "a+b" */
	H_RULE(NOT, h_choice(h_sequence(h_ch('+'), h_not(h_ch('+')), NULL), h_token((const uint8_t*)"++", 2), NULL));
	pp = h_sequence(h_ch('a'), NOT, h_ch('b'),h_end_p(),NULL);
}

/* Passing Tests: "a++b", "a+b" */
static void test_1() {
	input[0] = 'a';
	input[1] = '+';
	input[2] = '+';
	input[3] = 'b';
	g_check_parse_ok(pp, BKEND, input, 4);
}

static void test_2() {
	input[0] = 'a';
	input[1] = '+';
	input[2] = 'b';
	g_check_parse_ok(pp, BKEND, input, 3);
}

/* Failing Tests: "a+-b", "A++b", "a+Ab" */
static void test_3() {
	input[0] = 'a';
	input[1] = '+';
	input[2] = '-';
	input[3] = 'b';
	g_check_parse_failed(pp, BKEND, input, 4);
}

static void test_4() {
	input[0] = 'A';
	input[1] = '+';
	input[2] = '+';
	input[3] = 'b';
	g_check_parse_failed(pp, BKEND, input, 4);
}

static void test_5() {
	input[0] = 'a';
	input[1] = '+';
	input[2] = 'A';
	input[3] = 'b';
	g_check_parse_failed(pp, BKEND, input, 4);
}


void register_bug_tests() {
	g_test_add_func("/pass/test1",test_1);
	g_test_add_func("/pass/test2",test_2);
	g_test_add_func("/fail/test3",test_3);
	g_test_add_func("/fail/test4",test_4);
	g_test_add_func("/fail/test5",test_5);
}

int main(int argc, char *argv[]) {
	init_parser();
	g_test_init(&argc,&argv,NULL);
	register_bug_tests();
	return g_test_run();
}
