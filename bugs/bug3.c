/*
 * bug3 -- h_int16() issues using little endian
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

#define BKEND PB_LALR
//#define BKEND PB_MIN

const HParser *pp; /* the parser */

uint8_t input[1024]; /* the input is a buff of bytes */

/* The Parser */
void init_parser() {
	/* parse a single int16 between the values of -2 and 1 */
	H_RULE(littleint16, h_with_endianness(BYTE_LITTLE_ENDIAN,h_int16()));
	H_RULE(int16range, h_int_range(littleint16,-2,1));
	pp = h_sequence(int16range,h_end_p(),NULL);
}

/* Passing Tests: -2,-1,0,+1 */
static void test_n2() {
	input[0] = 0xFE; // 
	input[1] = 0xFF; // -2
	g_check_parse_ok(pp, BKEND, input, 2);
}

static void test_n1() {
	input[0] = 0xFF; // 
	input[1] = 0xFF; // -1
	g_check_parse_ok(pp, BKEND, input, 2);
}

static void test_zero() {
	input[0] = 0x00; // 0
	input[1] = 0x00; // 0
	g_check_parse_ok(pp, BKEND, input, 2);
}

static void test_p1() {
	input[0] = 0x01; 
	input[1] = 0x00; // +1
	g_check_parse_ok(pp, BKEND, input, 2);
}

/* Failing Tests: -3,+2,+1+1 */
static void test_n3() {
	input[0] = 0xFD; 
	input[1] = 0xFF; // -3
	g_check_parse_failed(pp, BKEND, input, 2);
}

static void test_p2() {
	input[0] = 0x02; 
	input[1] = 0x00; // +2
	g_check_parse_failed(pp, BKEND, input, 2);
}

static void test_2val() {
	input[0] = 0x01;
	input[1] = 0x00; // +1
	input[0] = 0x01; 
	input[1] = 0x00; // +1
	g_check_parse_failed(pp, BKEND, input, 4);
}


void register_bug_tests() {
	g_test_add_func("/pass/neg2",test_n2);
	g_test_add_func("/pass/neg1",test_n1);
	g_test_add_func("/pass/zero",test_zero);
	g_test_add_func("/pass/pos1",test_p1);

	g_test_add_func("/fail/neg3",test_n3);
	g_test_add_func("/fail/pos2",test_p2);
	g_test_add_func("/fail/2val",test_2val);
}

int main(int argc, char *argv[]) {
	init_parser();
	g_test_init(&argc,&argv,NULL);
	register_bug_tests();
	return g_test_run();
}
