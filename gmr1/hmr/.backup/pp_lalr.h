#pragma once

#include <stdio.h>
#include <string.h>
#include <hammer/hammer.h>
#include <hammer/glue.h>
#include <hammer/test_suite.h>
#include "../../../hammer/src/backends/lr.h"


/* 
 * pp_lalr(pn,parser)) -- given a parser name pn and a parser, compile
 * the parser with the LALR backend then output the grammar and
 * associated parsing table to the file pn.output
 *
 * returns 0 on success; non-zero otherwise
 */
int pp_lalr(char *pfn,HParser *hmr_parser);
