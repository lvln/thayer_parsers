#pragma once

#include <stdio.h>
#include <string.h>
#include <hammer/hammer.h>
#include <hammer/glue.h>
#include <hammer/test_suite.h>

/*
 * NOTE the file lr.h has to be installed from the hammer src directory -- not coppied by default
 */
#include <hammer/backends/lr.h>


/* 
 * pp_lalr(pn,parser)) -- given a parser name pn and a parser, compile
 * the parser with the LALR backend then output the grammar and
 * associated parsing table to the file pn.output
 *
 * returns 0 on success; non-zero otherwise
 */
int pp_xml(char *pfn,HParser *hmr_parser);
