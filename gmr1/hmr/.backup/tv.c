#include "tv.h"

/* Set the backend */
#define BKEND PB_LALR /* or PB_LALR, PB_MIN */

/* the test parser in use */
const HParser *test_parser;


/* 
 * success cases
 */
static void p1() {
	char *str = "a";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p2() {
	char *str = "()";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p3() {
	char *str = "(a)";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p4() {
	char *str = "((a))";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p5() {
	char *str = "(((((a)))))";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p6() {
	char *str = "a,a";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p7() {
	char *str = "a,a,a";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p8() {
	char *str = "a,(a);()";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p9() {
	char *str = "(a),(a)";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p10() {
	char *str = "a,a,a;a,a,a";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p11() {
	char *str = "a,a,(a,a,a,a;a,a,a,a;a,a,a,a);();(a,a;a)";
	g_check_parse_ok(test_parser,BKEND,str,strlen(str));    
}

static void p12() {
		char *str = "a,a,a,a;a,a,a,a;a,a,a,a";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p13() {
		char *str = "(a,a,a)";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p14() {
		char *str = "a,(a;a,a,a)";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p15() {
		char *str = "a,a,a,a,()";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p16() {
		char *str = "(a;a;a;a,a,a)";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p17() {
		char *str = "a,(),(a);a,a";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p18() {
		char *str = "a,a,a,(a;())";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p19() {
		char *str = "a,a,a;a;a,a,a,a;a,a";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p20() {
		char *str = "a;a;a;a,a,(a,a,a,a,a)";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p21() {
		char *str = "(a,a,a,a;a,a,a;a,a;a)";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p22() {
		char *str = "a,a,(a,a,a;a;a,a);a,a";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

static void p23() {
		char *str = "a,a,a,a,a;a,a,a,a,a";
		g_check_parse_ok(test_parser,BKEND,str,strlen(str));
}

/* 
 * failure cases 
 */
static void f1() {
	char *str = "aa";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f2() {
	char *str = ")";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f3() {
	char *str = "(aa)";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f4() {
	char *str = "(((a))";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f5() {
	char *str = "(a(a))";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f6() {
	char *str = "aaaa";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f7() {
	char *str = "";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f8() {
	char *str = ",";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f9() {
	char *str = "a,";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f10() {
	char *str = "a,x";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f11() {
	char *str = "a;();,";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f12() {
	char *str = "a,;";
	g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f13() {
		char *str = "ac";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f14() {
		char *str = "(a,a,,a)";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f15() {
		char *str = "(a,a,;a)";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f16() {
		char *str = "a,a,a,aa";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f17() {
		char *str = "a;a;a; a";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f18() {
		char *str = "((a;a,a,))";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f19() {
		char *str = "a,a,();;";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f20() {
		char *str = "a;a;a;,";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f21() {
		char *str = "a,a,a;a,a,(a,a,a,a,a";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f22() {
		char *str = "a;a,a,a;a,a,a;;a,a,a";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f23() {
		char *str = "(a,a;a,a,a,a;a);a,a,a,,a";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f24() {
		char *str = "a;a;a,a,a,a,aaa,a";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}

static void f25() {
		char *str = "a,a,a,(a;a),ab,a,a;ac";
		g_check_parse_failed(test_parser,BKEND,str,strlen(str));
}


void register_tests(const HParser *hmr_parser) {
	test_parser = hmr_parser;

	/* passing casess */
	g_test_add_func("/pass/p1",p1);
	g_test_add_func("/pass/p2",p2);
	g_test_add_func("/pass/p3",p3);
	g_test_add_func("/pass/p4",p4);
	g_test_add_func("/pass/p5",p5);
	g_test_add_func("/pass/p6",p6);
	g_test_add_func("/pass/p7",p7);
	g_test_add_func("/pass/p8",p8);
	g_test_add_func("/pass/p9",p9);
	g_test_add_func("/pass/p10",p10);
	g_test_add_func("/pass/p11",p11);
	g_test_add_func("/pass/p12",p12);
	g_test_add_func("/pass/p13",p13);
	g_test_add_func("/pass/p14",p14);
	g_test_add_func("/pass/p15",p15);
	g_test_add_func("/pass/p16",p16);
	g_test_add_func("/pass/p17",p17);
	g_test_add_func("/pass/p18",p18);
	g_test_add_func("/pass/p19",p19);
	g_test_add_func("/pass/p20",p20);
	g_test_add_func("/pass/p21",p21);
	g_test_add_func("/pass/p22",p22);
	g_test_add_func("/pass/p23",p23);
	/* failure cases */
	g_test_add_func("/fail/f1",f1);
	g_test_add_func("/fail/f2",f2);
	g_test_add_func("/fail/f3",f3);
	g_test_add_func("/fail/f4",f4);
	g_test_add_func("/fail/f5",f5);
	g_test_add_func("/fail/f6",f6);
	g_test_add_func("/fail/f7",f7);
	g_test_add_func("/fail/f8",f8);
	g_test_add_func("/fail/f9",f9);
	g_test_add_func("/fail/f10",f10);
	g_test_add_func("/fail/f11",f11);
	g_test_add_func("/fail/f12",f12);
	g_test_add_func("/fail/f13",f13);
	g_test_add_func("/fail/f14",f14);
	g_test_add_func("/fail/f15",f15);
	g_test_add_func("/fail/f16",f16);
	g_test_add_func("/fail/f17",f17);
	g_test_add_func("/fail/f18",f18);
	g_test_add_func("/fail/f19",f19);
	g_test_add_func("/fail/f20",f20);
	g_test_add_func("/fail/f21",f21);
	g_test_add_func("/fail/f22",f22);
	g_test_add_func("/fail/f23",f23);
	g_test_add_func("/fail/f24",f24);
	g_test_add_func("/fail/f25",f25);
}




