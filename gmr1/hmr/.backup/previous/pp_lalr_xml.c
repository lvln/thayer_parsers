/*
 * pp_lalr_xml.c --- Parser grammar xml generator
 *
 * Author: Stephen Taylor, Ellie Baker, Sarah Korb
 * Created: 10-21-2020
 * Version: 1.0
 *
 * Description: This file uses functions from the HAMMER library
 * to print the grammar rules, terminal symbols and non-terminal symbols
 * of a HAMMER parser
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pp_lalr.h"
#include "pp_lalr_xml.h"

//Rule structure holds a single rule
typedef struct ruleinfo {
    char *lhs;
    int rhslen;
    char *rhs[10];
} rule_t;


static void put_line(char *sp, int tabs, FILE *f) {
	int i;
	for(i=0;  i<tabs; i++)  
		fprintf(f, "  ");   /* put two spaces, tabs times */
    fprintf(f, "%s\n", sp);	
}

static void put_indent(char *sp, int tabs, FILE *f) {
	int i;
	for(i=0;  i<tabs; i++)  
		fprintf(f, "  ");   /* put two spaces, tabs times */
    fprintf(f, "%s", sp);	
}

static char *nonterminal_name(const HCFGrammar *g, const HCFChoice *nt)
{
  int i;
	static char buf[16] = {0};
  // find nt's number in g
  size_t n = (uintptr_t)h_hashtable_get(g->nts, nt);
  // NB the start symbol (number 0) is always "A".
  for(i=14; i>=0 && (n>0 || i==14); i--) {
    buf[i] = 'A' + n%26;
    n = n/26;   // shift one digit
  }
  return buf+i+1;
}
 
static char *nonterminal_namelhs(const HCFGrammar *g, const HCFChoice *nt)
{
  int i;
	static char buf[16] = {0};
  // find nt's number in g
  size_t n = (uintptr_t)h_hashtable_get(g->nts, nt);
  // NB the start symbol (number 0) is always "A".
  for(i=14; i>=0 && (n>0 || i==14); i--) {
    buf[i] = 'A' + n%26;
    n = n/26;   // shift one digit
  }
  return buf+i+1;
}

static char *nonterminal_namerhs(const HCFGrammar *g, const HCFChoice *nt)
{
  int i;
	static char buf[16] = {0};
  // find nt's number in g
  size_t n = (uintptr_t)h_hashtable_get(g->nts, nt);
  // NB the start symbol (number 0) is always "A".
  for(i=14; i>=0 && (n>0 || i==14); i--) {
    buf[i] = 'A' + n%26;
    n = n/26;   // shift one digit
  }
  return buf+i+1;
}

 
static HCFChoice **pprint_string(FILE *f, HCFChoice **x)
{
  for(; *x; x++) {
    if ((*x)->type != HCF_CHAR)
      break;
    else
      h_pprint_char(f, (*x)->chr); // NOT THE $ PRINT SPOT
  }
  return x;
}

static void h_pprint_symbolxml(FILE *f, const HCFGrammar *g, const HCFChoice *x)
{
  switch(x->type) {
  case HCF_END:
    fprintf(f, "$end");
    break;
  default:
    fputs(nonterminal_namerhs(g, x), f);
		break;
  }
}
 
 
static void h_pprint_sequence_xml(FILE *f, const HCFGrammar *g, const HCFSequence *seq)
{
  HCFChoice **x = seq->items;
  if (*x == NULL)
    put_line("<empty/>", 5, f);
  else {
    while(*x) {
      if (x != seq->items)
          fputc(' ', f); // internal separator
      
      if ((*x)->type == HCF_CHAR) {
          put_indent("<symbol>", 5, f);
          x = pprint_string(f, x);
          fprintf(f, "</symbol>\n");
      }
      else {
          put_indent("<symbol>", 5, f);
          h_pprint_symbolxml(f, g, *x); //function that prints the $
          fprintf(f, "</symbol>\n");
          x++;
      }
    }
  }
}

static void pprint_sequence_xml(FILE *f, const HCFGrammar *g, const HCFSequence *seq)
{
  h_pprint_sequence_xml(f, g, seq);
}
 
static int pprint_ntrules_xml(FILE *f, const HCFGrammar *g, const HCFChoice *nt, int rule_num)
{
  const char *name = nonterminal_namelhs(g, nt);
  char line[80];
  assert(nt->type == HCF_CHOICE);
  HCFSequence **p = nt->seq;
  if (*p == NULL)
      return -100;          // shouldn't happen

  for(; *p; p++) {
    //null circumstance printed here
    sprintf(line, "<rule number=\"%d\">", rule_num);
    put_line(line, 3, f);
    sprintf(line, "<lhs>%s</lhs>", name);
    put_line(line, 4, f);
    put_line("<rhs>", 4, f);
    pprint_sequence_xml(f, g, *p);
    put_line("</rhs>", 4, f);
    put_line("</rule>\n", 3, f);
    rule_num ++; //increment rule
  }
  return rule_num;
}

static void h_pprint_grammar_xml(FILE *fp, HCFGrammar *g, int indent, int rule_num)
{
  int len;
  size_t s;
  if (g->nts->used < 1)
    return;
   
  // determine maximum string length of symbol names
  for(len=1, s=26; s < g->nts->used; len++, s*=26);
  // iterate over g->nts
  size_t i;
  HHashTableEntry *hte;
  for(i=0; i < g->nts->capacity; i++) {
    for(hte = &g->nts->contents[i] ; hte; hte = hte->next) {
      if (hte->key == NULL)
        continue;
      else{
        const HCFChoice *a = hte->key;        // production's left-hand symbol
        assert(a->type == HCF_CHOICE);
        rule_num = pprint_ntrules_xml(fp, g, a, rule_num);
      }
    }
  }
}

static void h_pprint_charterminal(FILE *f, uint8_t c, int num)
{
/*part 1 of terminal fxn
 this function is modified to print terminal output */
  char line[80];
  sprintf(line, "<terminal symbol-number=\"%d\" token-number=\"%d\" name=\"\'%c\'\"/>", num, (unsigned int)c, c);
  put_line(line, 3, f);
}


static HCFChoice **pprint_stringterminal(FILE *f, HCFChoice **x, int num)
{
/*part 2 of terminal fxn
 modified for terminal output, when type portion of x structure is char, passed to be printed.
seems possible to eliminate this step and just pass *x->chr to charterminal but old method is preserved here*/
  for(; *x; x++) {
    if ((*x)->type != HCF_CHAR)
        break;
    else
      h_pprint_charterminal(f, (*x)->chr, num);
  }
  return x;
}

static int h_pprint_sequence_terminal(FILE *f, const HCFGrammar *g, const HCFSequence *seq, int num)
{
  HCFChoice **x = seq->items;
  /*
	 * part 3 of terminal fxn modified for terminal printing to only
	 * pass terminal symbols to print terminal fxns
	 */
  if (*x != NULL) {
    while(*x) {
      if ((*x)->type == HCF_CHAR) {
				x = pprint_stringterminal(f, x, num);
				num++;
      }
      else
				x++;
    }
  }
  return num;
}

static int pprint_sequence_terminal(FILE *f, const HCFGrammar *g, const HCFSequence *seq, int num)
{
  num = h_pprint_sequence_terminal(f, g, seq, num);
  return num;
}


static int pprint_t_xml(FILE *f, const HCFGrammar *g, const HCFChoice *nt, int num)
{
	/*
	 * part 5 of terminal fxn walks through grammar/characters within
	 * structure, modified to just call sequence terminal fxn and pass
	 * *p
	 */
  assert(nt->type == HCF_CHOICE);
  HCFSequence **p = nt->seq;
  if (*p == NULL)
      return -100;          // shouldn't happen
  for(; *p; p++)
      num = pprint_sequence_terminal(f, g, *p, num);
  return num;
}
 
static int h_pprint_terminal_xml(FILE *fp, HCFGrammar *g, int indent, int num)
{
  int len;
  size_t s;
  if (g->nts->used < 1)
		return -100;

  // determine maximum string length of symbol names
  for(len=1, s=26; s < g->nts->used; len++, s*=26);
  // iterate over g->nts
  size_t i;
  HHashTableEntry *hte;
	for(i=0; i < g->nts->capacity; i++) {
    for(hte = &g->nts->contents[i]; hte; hte = hte->next) {
      if (hte->key == NULL)
        continue;
      else{
        const HCFChoice *a = hte->key;        // production's left-hand symbol
        assert(a->type == HCF_CHOICE);
        num = pprint_t_xml(fp, g, a, num);
      }
    }
  }
  return num;
}
 
static void pprint_nt(FILE *f, const HCFGrammar *g, const HCFChoice *nt, int indent, int len, int num)
{
  char line[80];
  const char *name = nonterminal_name(g, nt);
  /* Prints non-terminals in the correct xml format using
   * 'nonteriminal_name' function to generate printable char object
  */
  sprintf(line,"<nonterminal symbol-number=\"%d\" name = \"%s\"/>", num, name);
  put_line(line, 3, f);
}

static void h_pprint_nonterminal(FILE *file, const HCFGrammar *g, int indent, int num)
{
  int len;
  size_t s;
  if (g->nts->used < 1)
		return;
  // determine maximum string length of symbol names
  for(len=1, s=26; s < g->nts->used; len++, s*=26);
  // iterate over g->nts
  size_t i;
  HHashTableEntry *hte;
  for(i=0; i < g->nts->capacity; i++) {
    for(hte = &g->nts->contents[i]; hte; hte = hte->next) {
      if (hte->key == NULL)
        continue;
			else{
        const HCFChoice *a = hte->key;        // production's left-hand symbol
        assert(a->type == HCF_CHOICE);
        pprint_nt(file, g, a, indent, len, num);
        num = num + 1;
      }
    }
  }
}

int pp_lalr_xml(FILE *fp, HCFGrammar *g, int indent) {
	int num = 0;
	int rule_num = 0;

	//printing xml rules
	put_line("<grammar>", 1, fp);
	put_line("<rules>", 2, fp);
	h_pprint_grammar_xml(fp, g, 0, rule_num);
	//printing xml grammar
	put_line("</rules>", 2, fp);
	put_line("<terminals>", 2, fp);
	num = h_pprint_terminal_xml(fp, g, 0, num);
	put_line("</terminals>", 2, fp);
	//printing non-terminals
	put_line("<nonterminals>", 2, fp);
	h_pprint_nonterminal(fp, g, 0, num);
	put_line("</nonterminals>", 2, fp);
	put_line("</grammar>", 1, fp);
	return 0;
 }
