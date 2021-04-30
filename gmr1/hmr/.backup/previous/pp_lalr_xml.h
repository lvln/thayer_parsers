#include "pp_lalr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * pp_lalr_xml(file,grammer, indent)) -- given an HCF grammar, file pointer
 * and an indent number, output the grammar and rules in xml to the file
 *
 * returns 0 on success
 */
int pp_lalr_xml(FILE *fp, HCFGrammar *g, int indent);
