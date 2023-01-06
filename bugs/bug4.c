/*
 * bug4 -- h_and() issues
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
	/* Check if string is hex value "0xA5", consume string if h_and passes */
	H_RULE(AND, h_sequence(h_and(h_token((const uint8_t*)"0x", 2)),h_token((const uint8_t*)"0xA5", 4),NULL));
	pp = h_sequence(AND,h_end_p(),NULL);
}

/* Passing Tests: "0xA5" */
static void test_1() {
	input[0] = '0';
	input[1] = 'x';
	input[2] = 'A';
	input[3] = '5';
	g_check_parse_ok(pp, BKEND, input, 4);
}

/* Failing Tests: "0xa5", "0x5A", "0A5" */
static void test_lowCaseA() {
	input[0] = '0';
	input[1] = 'x';
	input[2] = 'a';
	input[3] = '5';
	g_check_parse_failed(pp, BKEND, input, 4);
}

static void test_2() {
	input[0] = '0';
	input[1] = 'x';
	input[2] = '5';
	input[3] = 'A';
	g_check_parse_failed(pp, BKEND, input, 4);
}

static void test_badval() {
	input[0] = '0';
	input[2] = 'A';
	input[3] = '5';
	g_check_parse_failed(pp, BKEND, input, 3);
}


void register_bug_tests() {
	g_test_add_func("/pass/test1",test_1);
	g_test_add_func("/fail/lowercaseA",test_lowCaseA);
	g_test_add_func("/fail/test2",test_2);
	g_test_add_func("/fail/badval",test_badval);
}

int main(int argc, char *argv[]) {
	init_parser();
	g_test_init(&argc,&argv,NULL);
	register_bug_tests();
	return g_test_run();
}
