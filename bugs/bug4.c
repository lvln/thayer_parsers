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
	/* Check if byte is of hex value 0xA5, consume byte if h_and passes */
	H_RULE(AND, h_sequence(h_and(h_ch('\xA5')),h_ch('\xA5'),NULL));
	pp = h_sequence(AND,h_end_p(),NULL);
}

/* Passing Tests: 0xA5 */
static void test_val() {
	input[0] = 0xA5;
	g_check_parse_ok(pp, BKEND, input, 1);
}

/* Failing Tests: 0xFE,0x02,0x01 0x01 */
static void test_m2() {
	input[0] = 0xFE; // -2
	g_check_parse_failed(pp, BKEND, input, 1);
}

static void test_p2() {
	input[0] = 0x02; // +2
	g_check_parse_failed(pp, BKEND, input, 1);
}

static void test_2val() {
	input[0] = 0x01; // +1
	input[1] = 0x01; // +1
	g_check_parse_failed(pp, BKEND, input, 2);
}


void register_bug_tests() {
	g_test_add_func("/pass/val",test_val);
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
