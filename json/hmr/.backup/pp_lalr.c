#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pp_lalr.h"



//WORKING: a structure which holds a single rule
typedef struct ruleinfo {
	char *lhs;
	int rhslen;
	char *rhs[10];
} rule_t;

typedef struct expansioninfo {
	int min;
	int max;
	int rhs;
} expansion_t;



#define NUMCHARS 256

char *currlhs;
char *currrhs[10];
int currrhslen = 0;


//WORKING: this is the array where all rules are stored
rule_t *ruletable[1000];
expansion_t *rangetable[1000];
rule_t *enumerate[1000];
int rangetrack = 0;

rule_t *currrule;
expansion_t *currexp;

rule_t *endrule;

int number = 0;
int et = 0;
int rule_numstruct = 0;
int rule_add = 0;

//WORKING: keeps track of the place in the rhs array where we're defining/adding a character
int track = 0;
int track1 = 0;
int track2 = 0;

char str2[]= "      ";
char str4[] = "        ";
char str6[] = "          ";
char version[] = "1.0";
char rep_version[] = "1.0.0";
int symbol = 1;
int ready = 0;
int firsttime = 0;
int transitionyn = 0;
int reductionyn = 0;
int alert = 0;

static char str1state[]= "    ";
static char str2state[]= "      ";

//WORKING: rp must be universal so rhs char *[] can be added to as it is defined
rule_t *rp;
rule_t *enumd;

//WORKING: nextrule keeps track of which rule is being writing in the array ruletable
int nextrule=0;

int tester = 0;

int firstterm = 1;

int endred = 0;
int reductionmarker = 0;


static bool chartable[NUMCHARS];
int num = 0;




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

bool matchrule(rule_t *rp1, rule_t *rp2) {
	int i;
	if(strcmp(rp1->lhs,rp2->lhs)!=0) 
		return false;
	for(i=0; i<rp1->rhslen; i++) {
		if(rp1->rhs[i]==NULL && rp2->rhs[i]!=NULL) 
			return false;
		if(rp1->rhs[i]!=NULL && rp2->rhs[i]==NULL)
			return false;
		if(rp1->rhs[i]!=NULL && strcmp(rp1->rhs[i],rp2->rhs[i])!=0)
			return false;
	}
	return true;
}

int match_rule(rule_t *currentrule) {
	int i;
	rule_t *rtp;
	
	for(i=0; i<rule_numstruct; i++) {
		rtp = ruletable[i];
		 
		if(matchrule(currentrule,rtp)) {
			return i;
		}
	}
	return -1;
}





rule_t *make_empty_rule() {
	rule_t *rp;
	int i;
	
	if((rp = (rule_t*)malloc(sizeof(rule_t)))==NULL)
		return NULL;
	rp->lhs = NULL;
	rp->rhslen = 0;
	for(i=0; i<10; i++) {
		rp->rhs[i] = NULL;
	}
	return rp;
}

expansion_t *make_empty_expansion() {
	expansion_t *ep;
	
	if((ep = (expansion_t*)malloc(sizeof(expansion_t)))==NULL)
		return NULL;
	ep->min = 0;
	ep->max = 0;
	ep->rhs = 0;
	return ep;
}



//WORKING: in this function the lhs is assigned to a rule in ruletable
int put_rule(char *lhs) {
	if((rp = (rule_t*)malloc(sizeof(rule_t))) == NULL) {
		printf("error\n");
		return -1;
	}
	rp->lhs = lhs;
	
	return 0;
}

char *get_lhs(char *lhs) {
	
	char *lhscopy;
	
	if((lhscopy = (char*)malloc(strlen(lhs)+1))==NULL)
		return NULL;
	strcpy(lhscopy,lhs);
	return lhscopy;
}


char *nonterminal_namerhs(const HCFGrammar *g, const HCFChoice *nt)
{
  static char buf[16] = {0}; // 14 characters in base 26 are enough for 64 bits
	
  // find nt's number in g
  size_t n = (uintptr_t)h_hashtable_get(g->nts, nt);
	
  // NB the start symbol (number 0) is always "A".
  int i;
  for(i=14; i>=0 && (n>0 || i==14); i--) {
    buf[i] = 'A' + n%26;
    n = n/26;   // shift one digit
  }
	
  return buf+i+1;
}

char *nonterminal_name(const HCFGrammar *g, const HCFChoice *nt)
{
  static char buf[16] = {0}; // 14 characters in base 26 are enough for 64 bits
	
  // find nt's number in g
  size_t n = (uintptr_t)h_hashtable_get(g->nts, nt);
	
  // NB the start symbol (number 0) is always "A".
  int i;
  for(i=14; i>=0 && (n>0 || i==14); i--) {
    buf[i] = 'A' + n%26;
    n = n/26;   // shift one digit
  }
	
  return buf+i+1;
}



static void expansion_print(FILE *f) {
	int i;
	int e;
	char line[80];
	int first;
	int last;
    char lhsrange1[80];
	char *lhsrange;
	char rhsrange1[80];
	char *rhsrange;
	
	rule_add = rule_numstruct-1;
	for (i=0; i<rangetrack; i++) {
	  first = rangetable[i]->min;
	  last = rangetable[i]->max;
	  for (e=first; e<last+1; e++) {
	  sprintf(line, "rule %d:[%c-%c] ->%c", rule_numstruct, rangetable[i]->min, rangetable[(i)]->max, e);
	  sprintf(lhsrange1, "[%c-%c]", rangetable[i]->min, rangetable[(i)]->max);
	  sprintf(rhsrange1, "%c", e);
	  lhsrange = get_lhs(lhsrange1);
	  rhsrange = get_lhs(rhsrange1);
	  rp = make_empty_rule();
	  rp->lhs = lhsrange;
	  rp->rhs[0] = rhsrange;
	  rp->rhslen = 1;
	  ruletable[rule_numstruct]=rp;
	  rule_numstruct++;
      put_line(line, 0, f);
      }
	}
}


static void rangeprint(FILE *f, expansion_t *range) {
	int e;
	int repeat = 0;
	
	for (e=0; e<rangetrack; e++) {
	  if ((rangetable[e]->min == range->min) && rangetable[e]->max == range->max) {
	    repeat = 1;
	  }   
	}
	if (repeat == 0) {
	  rangetable[rangetrack] = range;
	  //fprintf(f, "\nADDED range %d, min %c, max %c\n", rangetrack, rangetable[rangetrack]->min, rangetable[rangetrack]->max);
	  rangetrack++;
	}
	
}

void h_pprint_charrule(FILE *f, uint8_t c)
{
  switch(c) {
  case '"': fputs("\\\"", f); break;
  case '\\': fputs("\\\\", f); break;
  case '\b': fputs("\\b", f); break;
  case '\t': 
  fputs("\\t", f); 
  //fprintf(f, "part two \\t");
  break;
  case '\n': fputs("\\n", f); break;
  case '\r': fputs("\\r", f); break;
  default:                       //HOW TO DEAL WITH THIS LONG TERM!!!!
      fputc(c, f);
      //fprintf(f, "   %c   ", c);
      break;
  }
}

static void pprint_charset_charrule(FILE *f, uint8_t c)
{
  switch(c) {
  case '"': fputc(c, f); break;
  case '-': fputs("\\-", f); break;
  case ']': fputs("\\-", f); break;
  default:  h_pprint_charrule(f, c);
  }
}

static void pprint_charsetrule(FILE *f, const HCharset cs)
{
  int i;
  expansion_t *range;
  
  range = make_empty_expansion();
  fputc('[', f);
  for(i=0; i<256; i++) {
    if (charset_isset(cs, i)) {
      pprint_charset_charrule(f, i);
        range->min=i;
      // detect ranges
      if (i+2<256 && charset_isset(cs, i+1) && charset_isset(cs, i+2)) {
        fputc('-', f);
        for(; i<256 && charset_isset(cs, i); i++);
        i--;  // back to the last in range
        range->max=i;
        pprint_charset_charrule(f, i);
        rangeprint(f, range);
      }
      else {
      switch(i) {
      case '"': fputs("\\\"", f); break;
      case '\\': fputs("\\\\", f); break;
      case '\b': fputs("\\b", f); break;
      case '\t': 
      fputs("\\t", f); 
      fprintf(f, "part two \\t");
      break;
      case '\n': fputs("\\n", f); break;
      case '\r': fputs("\\r", f); break;
      default:                       //HOW TO DEAL WITH THIS LONG TERM!!!!
      fputc(i, f);
      fprintf(f, "   %c   ", i);
      break;
  }
      
      
      }
    }
  }
  fputc(']', f);
}


void h_pprint_char(FILE *f, uint8_t c)
{
  switch(c) {
  case '"': fputs("\\\"", f); break;
  case '\\': fputs("\\\\", f); break;
  case '\b': fputs("\\b", f); break;
  case '\t': fputs("\\t", f); break;
  case '\n': fputs("\\n", f); break;
  case '\r': fputs("\\r", f); break;
  default:
      fputc(c, f);
  }
}

void h_pprint_symbolrule(FILE *f, const HCFGrammar *g, const HCFChoice *x)
{
  switch(x->type) {
  case HCF_CHAR:
    fputc('"', f);
    h_pprint_char(f, x->chr);
    fputc('"', f);
    break;
  case HCF_END:
    fprintf(f, "$end");
    break;
  case HCF_CHARSET:
    pprint_charsetrule(f, x->charset);
    break;
  default:
    fputs(nonterminal_name(g, x), f);
  }
}




static HCFChoice **pprint_string(FILE *f, HCFChoice **x)
{
  fputc('"', f);
  for(; *x; x++) {
    if ((*x)->type != HCF_CHAR) {
      break;
    }
    h_pprint_char(f, (*x)->chr);
  }
  fputc('"', f);
  return x;
}


void h_pprint_sequencerule(FILE *f, const HCFGrammar *g, const HCFSequence *seq)
{
  HCFChoice **x = seq->items;

  if (*x == NULL) {  // the empty sequence
    fputs("\"\"", f);
  } else {
    while(*x) {
      if (x != seq->items) {
	fputc(' ', f); // internal separator
      }
      if ((*x)->type == HCF_CHAR) {
        // condense character strings
        x = pprint_string(f, x);
      } else {
        h_pprint_symbolrule(f, g, *x);
        x++;
      }
    }
  }
}





//added


static void pprint_charset_char(FILE *f, uint8_t c)
{
  switch(c) {
  case '"': fputc(c, f); break;
  case '-': fputs("\\-", f); break;
  case ']': fputs("\\-", f); break;
  default:  h_pprint_char(f, c);
  }
}

static void pprint_charset(FILE *f, const HCharset cs)
{
  int i;

  fputc('[', f);
  for(i=0; i<256; i++) {
    if (charset_isset(cs, i)) {
      pprint_charset_char(f, i);

      // detect ranges
      if (i+2<256 && charset_isset(cs, i+1) && charset_isset(cs, i+2)) {
        fputc('-', f);
        for(; i<256 && charset_isset(cs, i); i++);
        i--;  // back to the last in range
        pprint_charset_char(f, i);
      }
    }
  }
  fputc(']', f);
}



static void h_pprint_symbolstate(FILE *f, const HCFGrammar *g, const HCFChoice *x)
{
  switch(x->type) {
  case HCF_END:
    fprintf(f, "$end");
    break;
  case HCF_CHARSET:
  	pprint_charset(f, x->charset);
  	break;	
  default:
    fputs(nonterminal_name(g, x), f);
  }
}

//end added

char *nonterminal_namelhscurr(const HCFGrammar *g, const HCFChoice *nt)
{
  static char buf[16] = {0}; // 14 characters in base 26 are enough for 64 bits
	
  // find nt's number in g
  size_t n = (uintptr_t)h_hashtable_get(g->nts, nt);
	
  // NB the start symbol (number 0) is always "A".
  int i;
  for(i=14; i>=0 && (n>0 || i==14); i--) {
    buf[i] = 'A' + n%26;
    n = n/26;   // shift one digit
  }
	
  return buf+i+1;
}



void h_pprint_charstruct(FILE *f, uint8_t c)
{
  char *rhs;
  char *rhscopy;
  static char buff[1];
  buff[0]=c;
  buff[1]='\0';
  rhs = buff;
  if((rhscopy = (char*)malloc(strlen(rhs)+1))==NULL)
		printf("error");
	// return NULL;
  strcpy(rhscopy,rhs);
  rp->rhs[track]=rhscopy;
  track++;
}


static HCFChoice **pprint_stringstruct(FILE *f, HCFChoice **x)
{
	
  for(; *x; x++) {
    if ((*x)->type != HCF_CHAR) {
      break;
    }
    h_pprint_charstruct(f, (*x)->chr);
  }
  return x;
}






static void pprint_charsetstruct(FILE *f, const HCharset cs)
{
  int i;
  int minstruct;
  int maxstruct;
  char rangestruct1[80];
  char *rangestruct;

  

  for(i=0; i<256; i++) {
    if (charset_isset(cs, i)) {
      //pprint_charset_charrule(f, i);
        minstruct=i;
      // detect ranges
      if (i+2<256 && charset_isset(cs, i+1) && charset_isset(cs, i+2)) {
 
        for(; i<256 && charset_isset(cs, i); i++);
        i--;  // back to the last in range
        maxstruct=i;
        sprintf(rangestruct1, "[%c-%c]", minstruct, maxstruct);
        rangestruct = get_lhs(rangestruct1);
        rp->rhs[track]=rangestruct;
      }
      else {
        switch(i) {
  case '"': 
  sprintf(rangestruct1, "\\\"");
  break;
  case '\\':
  sprintf(rangestruct1, "\\\\");
  break;
  case '\b':
  sprintf(rangestruct1, "\\b");
  break;
  case '\t': 
  sprintf(rangestruct1, "\\t");
  break;
  case '\n':
  sprintf(rangestruct1, "\\n");
  break;
  case '\r': 
  sprintf(rangestruct1, "\\r");
  break;
  default:                       //HOW TO DEAL WITH THIS LONG TERM!!!!
      sprintf(rangestruct1, "%c", i);
      }
        rangestruct = get_lhs(rangestruct1);
        rp->rhs[track]=rangestruct;
        enumd->rhs[et]=rangestruct;
        et++;
        track++;
      }
    }
  }
}


static void h_pprint_symbolstruct(FILE *f, const HCFGrammar *a, const HCFChoice *z)
{
	char *ntrhs;
	switch(z->type) {
	case HCF_CHARSET:
  	pprint_charsetstruct(f, z->charset);
  	break;	
  default:
	ntrhs = nonterminal_namerhs(a, z);
	rp->rhs[track] = get_lhs(ntrhs);
    }
	track++;
	tester++;
}



void h_pprint_sequencestruct(FILE *file, const HCFGrammar *a, const HCFSequence *seg)
{
  HCFChoice **x = seg->items;
	
  if (*x == NULL) {  // the empty sequence
  }
	else {
    while(*x) {
			
			if(*x == NULL) {
        rp->rhs[track] = '\0';
			}
			else {
				if (x != seg->items) {
				}
				if ((*x)->type == HCF_CHAR) {
					//here's the rhs term
					x = pprint_stringstruct(file, x);
					
				}
				else {
					//here's the rhs nt
					h_pprint_symbolstruct(file, a, *x);
					x++;
        }
      }
    }
  }
}

void pprint_structure(FILE *f, const HCFGrammar *g, const HCFChoice *nt,
										int indent, int len)
{
  char *name;
	
  rp = make_empty_rule();
  enumd = make_empty_rule();
  name = nonterminal_name(g, nt);
	
  rp->lhs = get_lhs(name);
	
  assert(nt->type == HCF_CHOICE);
  HCFSequence **p = nt->seq;
  if (*p == NULL) {
    return;          // shouldn't happen
  }
	
  //RHS
  h_pprint_sequencestruct(f, g, *p++);    // print first production on the same line
	
  //WORKING: the rule has been completely defined- add to the ruletable at location nextrule
  enumd->rhslen = et;
  enumd->lhs = "no";
  ruletable[nextrule] = rp;
  enumerate[number] = enumd;
	
  //WORKING: now, increment nextrule to add another rule to the array
  nextrule++;
  if (et > 0) {
    number++;
    et = 0;
  }
	
  for(; *p; p++) {                // print the rest below with "or" bars
    rp = make_empty_rule();
    enumd = make_empty_rule();
    track = 0; //new rhs definition coming
    tester = 0;
		
    //LHS
    rp->lhs = get_lhs(name);
		
    //RHS
    h_pprint_sequencestruct(f, g, *p);
		
	enumd->rhslen = et;
	enumd->lhs = "no";
    //WORKING: the rule has been completely defined- add to the ruletable at location nextrule
    ruletable[nextrule] = rp;
	enumerate[number] = enumd;	
		
    //WORKING: now, increment nextrule to add another rule to the array
    nextrule++;
    if (et > 0) {
    number++;
    et = 0;
    }
		
		
  }
}


void h_pprint_structure(FILE *file, const HCFGrammar *g, int indent)
{
  if (g->nts->used < 1) {
    return;
  }
	
  // determine maximum string length of symbol names
  int len;
  size_t s;
  for(len=1, s=26; s < g->nts->used; len++, s*=26);
	
  // iterate over g->nts
  size_t i;
  HHashTableEntry *hte;
  for(i=0; i < g->nts->capacity; i++) {
    for(hte = &g->nts->contents[i]; hte; hte = hte->next) {
      if (hte->key == NULL) {
        continue;
      }
      const HCFChoice *a = hte->key;        // production's left-hand symbol
      assert(a->type == HCF_CHOICE);
			
      pprint_structure(file, g, a, indent, len);
      track = 0; //resets for each new rule
      tester = 0;
    }
  }
}



void pprint_lractionstate(FILE *f, const HCFGrammar *g, const HLRAction *action)
{
	//terminal printing shift state function
  switch(action->type) {
  case HLR_SHIFT:
    if(action->nextstate == HLR_SUCCESS)
      fputs("s~", f);
    else
      fprintf(f, " state =\"%zu\"/>\n", action->nextstate);
    break;
  case HLR_REDUCE:
    fputs("r(", f);
    h_pprint_symbolstate(f, g, action->production.lhs);
    fputs(" -> ", f);
#ifdef NDEBUG
    // if we can't print the production, at least print its length
    fprintf(f, "[%zu]", action->production.length);
#else
    HCFSequence seq = {action->production.rhs};
    h_pprint_sequence(f, g, &seq);
		
#endif
    fprintf(f, ")/>\n");
    break;
  case HLR_CONFLICT:
    fputc('!', f);
    for(HSlistNode *x=action->branches->head; x; x=x->next) {
      HLRAction *branch = x->elem;
      assert(branch->type != HLR_CONFLICT); // no nesting
      pprint_lractionstate(f, g, branch);
    }
    break;
  default:
    assert_message(0, "not reached");
  }
}

void pprint_lraction(FILE *f, const HCFGrammar *g, const HLRAction *action)
{
  switch(action->type) {
  case HLR_SHIFT:
    if(action->nextstate == HLR_SUCCESS)
      fputs("s~", f);
    else
      fprintf(f, "s%zu", action->nextstate);
    break;
  case HLR_REDUCE:
    fputs("r(", f);
    h_pprint_symbolstate(f, g, action->production.lhs);
    fputs(" -> ", f);
#ifdef NDEBUG
    // if we can't print the production, at least print its length
    fprintf(f, "[%zu]", action->production.length);
#else
    HCFSequence seq = {action->production.rhs};
    h_pprint_sequence(f, g, &seq);
#endif
    fputc(')', f);
    break;
  case HLR_CONFLICT:
    fputc('!', f);
    for(HSlistNode *x=action->branches->head; x; x=x->next) {
      HLRAction *branch = x->elem;
      assert(branch->type != HLR_CONFLICT); // no nesting
      pprint_lraction(f, g, branch);
    }
    break;
  default:
    assert_message(0, "not reached");
  }
}



static char *pprint_charsetreductionstruct(FILE *f, const HCharset cs)
{
  int i;
  int minrange;
  int maxrange;
  char range1[80];
  char *range0;
  
  currrule = make_empty_rule();
  for(i=0; i<256; i++) {
    if (charset_isset(cs, i)) {
      minrange=i;
        //currrule->min=i;
      // detect ranges
      if (i+2<256 && charset_isset(cs, i+1) && charset_isset(cs, i+2)) {
        for(; i<256 && charset_isset(cs, i); i++);
        i--;  // back to the last in range
        maxrange=i;
        sprintf(range1, "[%c-%c]", minrange, maxrange);
        range0 = get_lhs(range1);
        currrule->lhs=range0;
      }
    }
  }
 return range0;
}


char *h_pprint_symbolreduction(FILE *f, const HCFGrammar *a, const HCFChoice *z)
{
	char* lhs;
	if (z->type == HCF_CHARSET) {
	  lhs = pprint_charsetreductionstruct(f, z->charset);
	  alert = 1;
	}
	else
	  lhs = nonterminal_namelhscurr(a, z);
	
	return lhs;
}

void h_pprint_charred(FILE *f, uint8_t c)
{
	char *rhs;
	char *rhscopy;
	char rhsrange[80];
	char *rhsrangecp;
	
  static char buff[1];
  buff[0]=c;
  buff[1]='\0';
  rhs = buff;
  if((rhscopy = (char*)malloc(strlen(rhs)+1))==NULL)
		printf("problem");
	//  return NULL;
  strcpy(rhscopy,rhs);
  currrule->rhs[track2]=rhscopy;
  track2++;
  if (alert==1) {
    sprintf(rhsrange, "%c", c);
    rhsrangecp=get_lhs(rhsrange);
    currrule->rhs[0] = rhsrangecp;
    currrule->rhslen = 1;
  }
}


static HCFChoice **pprint_stringred(FILE *f, HCFChoice **x)
{
	
  for(; *x; x++) {
    if ((*x)->type != HCF_CHAR) {
      break;
    }
    h_pprint_charred(f, (*x)->chr);
		
  }
  return x;
}

char *nonterminal_namerhscurr(const HCFGrammar *g, const HCFChoice *nt)
{
  static char buf[16] = {0}; // 14 characters in base 26 are enough for 64 bits
	
  // find nt's number in g
  size_t n = (uintptr_t)h_hashtable_get(g->nts, nt);
	
  // NB the start x (number 0) is always "A".
  int i;
  for(i=14; i>=0 && (n>0 || i==14); i--) {
    buf[i] = 'A' + n%26;
    n = n/26;   // shift one digit
  }
	
  return buf+i+1;
}

static void pprint_charsetreductionstructrhs(FILE *f, const HCharset cs)
{
  int i;
  int minrange;
  int maxrange;
  char range1[80];
  char *range0;
  
  for(i=0; i<256; i++) {
    if (charset_isset(cs, i)) {
      minrange=i;
        //currrule->min=i;
      // detect ranges
      if (i+2<256 && charset_isset(cs, i+1) && charset_isset(cs, i+2)) {
        for(; i<256 && charset_isset(cs, i); i++);
        i--;  // back to the last in range
        maxrange=i;
        sprintf(range1, "[%c-%c]", minrange, maxrange);
        range0 = get_lhs(range1);
        currrule->rhs[track2]=range0;
      }
    }
  }
}



void h_pprint_symbolred(FILE *f, const HCFGrammar *a, const HCFChoice *z)
{
	char *ntrhs;
	if (z->type == HCF_CHARSET) {
      pprint_charsetreductionstructrhs(f, z->charset);
      alert = 1;
    }
    else {
	ntrhs = nonterminal_namerhscurr(a, z);
	currrule->rhs[track2] = get_lhs(ntrhs);
	}
	track2++;
	
}


void h_pprint_sequencereduction(FILE *file, const HCFGrammar *a, const HCFSequence *seg)
{
  HCFChoice **x = seg->items;
	
  if (*x == NULL) {  // the empty sequence
  } else {
    while(*x) {
      if (x != seg->items) {
      }
      if ((*x)->type == HCF_CHAR) {
        // condense character strings
        x = pprint_stringred(file, x);
      } else {
        h_pprint_symbolred(file, a, *x);
        x++;
      }
    }
  }
}


void pprint_reduction(FILE *f, const HCFGrammar *g, const HLRAction *action)
{
  char *temp;
  int rulenum;
	
  switch(action->type) {
  case HLR_REDUCE:
		
		currrule = make_empty_rule();
		
		//WORKING: logging the LHS
		
		temp = h_pprint_symbolreduction(f, g, action->production.lhs);
		currrule->lhs = get_lhs(temp);
		
		
		
		
#ifdef NDEBUG
    // if we can't print the production, at least print its length
    fprintf(f, "[%zu]", action->production.length);
#else
		
    //WORKING: logging the RHS
		
    HCFSequence seqred = {action->production.rhs};
    h_pprint_sequencereduction(f, g, &seqred);
    if (alert==0)
      currrule->rhslen = track2;
    track2 = 0;
	if (alert == 1) {
	   
	  rulenum = match_rule(currrule);
	}
	else 
      rulenum = match_rule(currrule);
		
		fprintf(f, "        <reductions>\n");
    fprintf(f,"          <reduction symbol= \"$default\" rule=\"%d\"/>\n", rulenum);
    reductionyn = 1;
    reductionmarker = 1;
    alert = 0;
		
		
#endif
    break;
  case HLR_CONFLICT:
    fputc('!', f);
    for(HSlistNode *x=action->branches->head; x; x=x->next) {
      HLRAction *branch = x->elem;
      assert(branch->type != HLR_CONFLICT); // no nesting
      pprint_lraction(f, g, branch);
    }
    break;
  default:
    assert_message(0, "not reached");
  }
}


static void valprint_lractionstate(FILE *file, void *env, void *val)
{
  const HLRAction *action = val;
  const HCFGrammar *grammar = env;
  if (action->type != HLR_REDUCE) {
		pprint_lractionstate(file, grammar, action);
  }
  else {
  }
}

static void valprint_lraction(FILE *file, void *env, void *val)
{
  const HLRAction *action = val;
  const HCFGrammar *grammar = env;
  pprint_lraction(file, grammar, action);
	
}


static void pprint_lrtable_terminals(FILE *file, const HCFGrammar *g,
                                     const HStringMap *map)
{
	
  h_pprint_stringmap(file, ' ', valprint_lraction, (void *)g, map);
}



void h_pprint_lrtable(FILE *f, const HCFGrammar *g, const HLRTable *table,
                      unsigned int indent)
{
  for(size_t i=0; i<table->nrows; i++) {
    for(unsigned int j=0; j<indent; j++) fputc(' ', f);
    fprintf(f, "%4zu:", i);
    if(table->forall[i]) {
      fputc(' ', f);
      pprint_lraction(f, g, table->forall[i]);
      if(!h_lrtable_row_empty(table, i))
        fputs(" !!", f);
    }
    H_FOREACH(table->ntmap[i], HCFChoice *symbol, HLRAction *action)
      fputc(' ', f);    // separator
		h_pprint_symbolstate(f, g, symbol);
		fputc(':', f);
		pprint_lraction(f, g, action);
    H_END_FOREACH
			fputc(' ', f);    // separator
    pprint_lrtable_terminals(f, g, table->tmap[i]);
    fputc('\n', f);
  }
}



void h_pprint_statesymbol(FILE *f, const HCFGrammar *g, const HCFChoice *x)
{
  char symbol;
  if(firsttime==0) {
    fprintf(f, "        <transitions>\n");
	}
  switch(x->type) {
  case HCF_CHAR:
    fprintf(f, "          <transition type = \"goto\" symbol = \"%c\" ", x->chr);
    transitionyn = 1;
    firstterm = 0;
    break;
  case HCF_END:
    fputc('!', f);
    break;
    break;
  default:
    symbol = *nonterminal_name(g, x);
    fprintf(f, "          <transition type = \"goto\" symbol = \"%c\"", symbol);
    transitionyn = 1;
    firstterm = 0;
    firsttime = 1;
  }
}


void pprint_stateaction(FILE *f, const HCFGrammar *g, const HLRAction *action) //only for nt
{
  switch(action->type) {
  case HLR_SHIFT:
    if(action->nextstate == HLR_SUCCESS)
      fprintf(f, "state =\"$accept\"/>\n");
    else
      fprintf(f, "state =\"%zu\"/>\n", action->nextstate);
    break;
#ifdef NDEBUG
    // if we can't print the production, at least print its length
    fprintf(f, "[%zu]", action->production.length);
#else
		
#endif
    fputc(')', f);
    break;
  case HLR_CONFLICT:
    fputc('!', f);
    for(HSlistNode *x=action->branches->head; x; x=x->next) {
      HLRAction *branch = x->elem;
      assert(branch->type != HLR_CONFLICT);
      pprint_lraction(f, g, branch);
    }
    break;
  default:
    assert_message(0, "not reached");
  }
}



#define BUFSIZE 512

static bool
pprint_stringmap_elemsstate(FILE *file, bool first, char *prefix, size_t n, char sep,
										void (*valprint)(FILE *f, void *env, void *val), void *env,
										const HStringMap *map)
{
  assert(n < BUFSIZE-4);
	
  if (map->epsilon_branch) { //this section prints the state = --- section for the terminal symbols
		
	}
	
  if (map->end_branch) { //this if doesn't seem to occur for us
    if (!first) {
    }
    first=false;
    if (n>0) {
    }
    fwrite(prefix, 1, n, file);
    if (n>0) {
    }
		
		void *longshot = map->end_branch;
		const HLRAction *actiontest = longshot;
		if(actiontest->type == HLR_REDUCE) { //will this work for the larger sense/other grammars or not...
			endred = 1;
			
			
			
			char *tempend;
			const HCFGrammar *grammarend = env;
			endrule = make_empty_rule();
			tempend = h_pprint_symbolreduction(file, grammarend, actiontest->production.lhs);
			endrule->lhs = get_lhs(tempend);
			
			HCFSequence seqend = {actiontest->production.rhs};
			h_pprint_sequencereduction(file, grammarend, &seqend);
			
			
			track2++;
			endrule->rhslen = track2;
			track2 = 0;
			
			
			
			
		}
		else {
			if(firstterm == 1) {
				fprintf(file, "        <transitions>\n"); //this is the issue spot
			}
			fprintf(file, "          <transition type = \"shift\" symbol = \"$end\""); //where end of input dollar is printed
			transitionyn = 1;
      firstterm = 0;
		}
    if (valprint) {
      valprint(file, env, map->end_branch);
    }
  }
	
	
  HHashTable *ht = map->char_branches;
  size_t i;
  HHashTableEntry *hte;
  for(i=0; i < ht->capacity; i++) {
    for(hte = &ht->contents[i]; hte; hte = hte->next) {
      if (hte->key == NULL) {
        continue;
      }
			uint8_t c = key_char((HCharKey)hte->key);
      HStringMap *ends = hte->value;
			
			
      size_t n_ = n;
      switch(c) {
      case '$':  prefix[n_++] = '\\'; prefix[n_++] = '$'; break;//this is not used
      case '"':  prefix[n_++] = '\\'; prefix[n_++] = '"'; break;
      case '\\': prefix[n_++] = '\\'; prefix[n_++] = '\\'; break;
      case '\b': prefix[n_++] = '\\'; prefix[n_++] = 'b'; break;
      case '\t': prefix[n_++] = '\\'; prefix[n_++] = 't'; break;
      case '\n': prefix[n_++] = '\\'; prefix[n_++] = 'n'; break;
      case '\r': prefix[n_++] = '\\'; prefix[n_++] = 'r'; break;
      default:
				
				symbol = c;
				
				
				
			}
			
      first = pprint_stringmap_elemsstate(file, first, prefix, n_,
										sep, valprint, env, ends);
    }
  }
	
	//this section prints the state = --- section for the terminal symbols
	
  if (map->epsilon_branch) {
		
		void *val = map->epsilon_branch;
		const HLRAction *action = val;
		if (action->type != HLR_REDUCE)
			
			{
				if(firstterm == 1) {
          fprintf(file, "        <transitions>\n");
				}
				fprintf(file, "          <transition type = \"shift\" symbol = \"\'%c\'\"", symbol);
				transitionyn = 1;
				
				if (!first) {
					
				}
				first=false;
				if (n==0) {
					
				} else {
					
					
				}
				
				if (valprint) {
					valprint(file, env, map->epsilon_branch); //calls pprint_lractionstate
					
				}
			}
    else {
			ready = 1;
			char *temp;
			const HCFGrammar *grammar = env;
			currrule = make_empty_rule();
			temp = h_pprint_symbolreduction(file, grammar, action->production.lhs);
			currrule->lhs = get_lhs(temp);
			
			
			
			HCFSequence seq = {action->production.rhs};
			h_pprint_sequencereduction(file, grammar, &seq);
			track2++;
			currrule->rhslen = track2;
			track2 = 0;
			
			
			
			
    }
	}
	
  return first;
	
}

void h_pprint_stringmapstate(FILE *file, char sep,
										 void (*valprint)(FILE *f, void *env, void *val), void *env,
										 const HStringMap *map)
{
  char buf[BUFSIZE];
  pprint_stringmap_elemsstate(file, true, buf, 0, sep, valprint, env, map);
}



static void pprint_state_terminals(FILE *file, const HCFGrammar *g,
										 const HStringMap *map)
{
	
  h_pprint_stringmapstate(file, ' ', valprint_lractionstate, (void *)g, map);
}




void h_pprint_state(FILE *f, const HCFGrammar *g, const HLRTable *table,
										unsigned int indent)
{
	
  fprintf(f, "\n  <automaton>\n\n");
  for(size_t i=0; i<table->nrows; i++) {
		
    fprintf(f, "%s<state number = \"%zu\">\n%s<actions>\n", str1state, i, str2state);
		reductionmarker = 0;
    if(table->ntmap[i] != NULL){
			
			H_FOREACH(table->ntmap[i], HCFChoice *symbol, HLRAction *action)
				fputc(' ', f);    // separator
      h_pprint_statesymbol(f, g, symbol);
      fputc(' ', f);
      pprint_stateaction(f, g, action);
			H_END_FOREACH
				fputc(' ', f);    // separator
			
			//WORKING: prints the terminal character state info
			pprint_state_terminals(f, g, table->tmap[i]);
			if(transitionyn == 0) {
        fprintf(f, "        <transitions/>\n");
        transitionyn = 0;
			}
			else {
				fprintf(f, "        </transitions>\n");
				transitionyn = 0;
			}
			firsttime = 0;
			
      if(table->forall[i]) {
				//fputc(' ', f);
				pprint_reduction(f, g, table->forall[i]);
				if(!h_lrtable_row_empty(table, i))
					fputs(" !!", f);
			}
			if (ready == 1) {
        int rulematch = match_rule(currrule);
        fprintf(f, "        <reductions>\n");
        fprintf(f, "          <reduction symbol= \"$default\" rule=\"%d\"/>\n", rulematch);
        reductionyn=1;
        ready = 0;
			}
			if (endred == 1 && reductionmarker == 0) {
        endred = 0;
			}
			if(reductionyn == 1) {
				fprintf(f, "        </reductions>\n      </actions>\n    </state>\n\n");
			}
			else {
				fprintf(f, "        <reductions/>\n      </actions>\n    </state>\n\n");
			}
			reductionyn = 0;
			firstterm = 1;
    }
  }
}




static void pprint_ruletable(FILE *f, const HCFGrammar *g, const HCFChoice *nt,
										 int indent, int len)
{
  int i;
  int column = indent + len;
	
  const char *name = nonterminal_name(g, nt);
	fprintf( f, "rule %d: ", rule_numstruct);
  // print rule head (symbol name)
  for(i=0; i<indent; i++) fputc(' ', f);
  fputs(name, f);
  i += strlen(name);
  for(; i<column; i++) fputc(' ', f);
  fputs(" ->", f);
	
  assert(nt->type == HCF_CHOICE);
  HCFSequence **p = nt->seq;
  if (*p == NULL) {
    return;          // shouldn't happen
  }
  h_pprint_sequencerule(f, g, *p++);    // print first production on the same line
  fprintf(f, "\n");
  rule_numstruct ++;
  //INCREMENT SPOT TO WRITE TO
	
  for(; *p; p++) {                // print the rest below with "or" bars
		fprintf( f, "rule %d: ", rule_numstruct);
    fputs(name, f);
    fputs(" -> ", f);
    h_pprint_sequencerule(f, g, *p);
    fprintf(f, "\n");
    rule_numstruct ++;
    //INCREMENT SPOT TO WRITE TO
  }
}



void h_pprint_ruletable(FILE *file, const HCFGrammar *g, int indent)
{
  if (g->nts->used < 1) {
    return;
  }
	
  // determine maximum string length of symbol names
  int len;
  size_t s;
  for(len=1, s=26; s < g->nts->used; len++, s*=26);
	
  // iterate over g->nts
  size_t i;
  HHashTableEntry *hte;
  for(i=0; i < g->nts->capacity; i++) {
    for(hte = &g->nts->contents[i]; hte; hte = hte->next) {
      if (hte->key == NULL) {
        continue;
      }
      const HCFChoice *a = hte->key;        // production's left-hand symbol
      assert(a->type == HCF_CHOICE);
			
      pprint_ruletable(file, g, a, indent, len);
    }
  }
}







static void h_pprint_range(FILE *f)
{
  char line[80];
  int e;
  
  for(e=nextrule; e<rule_numstruct; e++) {
    //null circumstance printed here
    sprintf(line, "<rule number=\"%d\">", e);
    put_line(line, 3, f);
    sprintf(line, "<lhs>%s</lhs>", ruletable[e]->lhs);
    put_line(line, 4, f);
    put_line("<rhs>", 4, f);
    put_indent("<symbol>", 5, f);
    sprintf(line, "%s", ruletable[e]->rhs[0]);
    put_indent(line, 0, f);
    fprintf(f, "</symbol>\n");
    put_line("</rhs>", 4, f);
    put_line("</rule>\n", 3, f);
  }
}

static void h_pprint_enumerated(FILE *f)
{
  char line[80];
  int e;
  int i;
  int a;
  
  for(e=0; e<number; e++) {
    for (a=0; a<enumerate[e]->rhslen; a++) {
    //null circumstance printed here
    sprintf(line, "<rule number=\"%d\">", rule_numstruct);
    put_line(line, 3, f);
    sprintf(line, "<lhs>[");
    put_indent(line, 4, f);
    for (i=0; i<enumerate[e]->rhslen; i++) {
       fprintf(f, "%s", enumerate[e]->rhs[i]);
    }
    fprintf(f, "]<lhs>\n");
    put_line("<rhs>", 4, f);
    put_indent("<symbol>", 5, f);
    fprintf(f, "%s", enumerate[e]->rhs[a]);
    fprintf(f, "</symbol>\n");
    put_line("</rhs>", 4, f);
    put_line("</rule>\n", 3, f);
    rule_numstruct++;
  }
}
}







static void init() {
    int i;
	for(i=0; i<NUMCHARS; i++)
	  chartable[i] = false;
}

static void fill(uint8_t c) {
	chartable[c]=true;
}

static char *nonterminal_namerules(const HCFGrammar *g, const HCFChoice *nt)
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
 
static char *nonterminal_namelhsrules(const HCFGrammar *g, const HCFChoice *nt)
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

static char *nonterminal_namerhsrules(const HCFGrammar *g, const HCFChoice *nt)
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

static void pprint_charset_charrules(FILE *f, uint8_t c)
{
  switch(c) {
  case '"': fputc(c, f); break;
  case '-': fputs("\\-", f); break;
  case ']': fputs("\\-", f); break;
  default:  h_pprint_char(f, c);
  }
}

static void pprint_charsetrules(FILE *f, const HCharset cs)
{
  int i;

  fputc('[', f);
  for(i=0; i<256; i++) {
    if (charset_isset(cs, i)) {
      pprint_charset_charrules(f, i);

      // detect ranges
      if (i+2<256 && charset_isset(cs, i+1) && charset_isset(cs, i+2)) {
        fputc('-', f);
        for(; i<256 && charset_isset(cs, i); i++);
        i--;  // back to the last in range
        pprint_charset_charrules(f, i);
      }
    }
  }
  fputc(']', f);
}

 
static HCFChoice **pprint_stringrules(FILE *f, HCFChoice **x)
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
  case HCF_CHARSET:
     pprint_charsetrules(f, x->charset);
     break;
  default:
    fputs(nonterminal_namerhsrules(g, x), f);
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
          x = pprint_stringrules(f, x);
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
  const char *name = nonterminal_namelhsrules(g, nt);
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

static void h_pprint_charterminal(FILE *f, uint8_t c)
{
/*part 1 of terminal fxn
 this function is modified to print terminal output */
  char line[80];
  if (chartable[c]== false) {
    sprintf(line, "<terminal symbol-number=\"%d\" token-number=\"%d\" name=\"\'%c\'\"/>", num, (unsigned int)c, c);
    num++;
    put_line(line, 3, f);
    fill(c);
  }
}


static HCFChoice **pprint_stringterminal(FILE *f, HCFChoice **x)
{
/*part 2 of terminal fxn
 modified for terminal output, when type portion of x structure is char, passed to be printed.
seems possible to eliminate this step and just pass *x->chr to charterminal but old method is preserved here*/
  for(; *x; x++) {
    if ((*x)->type != HCF_CHAR)
        break;
    else
      h_pprint_charterminal(f, (*x)->chr);
  }
  return x;
}

static void h_pprint_sequence_terminal(FILE *f, const HCFGrammar *g, const HCFSequence *seq)
{
  HCFChoice **x = seq->items;
  /*
	 * part 3 of terminal fxn modified for terminal printing to only
	 * pass terminal symbols to print terminal fxns
	 */
  if (*x != NULL) {
    while(*x) {
      if ((*x)->type == HCF_CHAR) {
				x = pprint_stringterminal(f, x);
      }
      else
				x++;
    }
  }
}

static void pprint_sequence_terminal(FILE *f, const HCFGrammar *g, const HCFSequence *seq)
{
  h_pprint_sequence_terminal(f, g, seq);
}


static void pprint_t_xml(FILE *f, const HCFGrammar *g, const HCFChoice *nt)
{
	/*
	 * part 5 of terminal fxn walks through grammar/characters within
	 * structure, modified to just call sequence terminal fxn and pass
	 * *p
	 */
  assert(nt->type == HCF_CHOICE);
  HCFSequence **p = nt->seq;
  if (*p == NULL)
    printf("error");
  for(; *p; p++)
      pprint_sequence_terminal(f, g, *p);
}
 
static void h_pprint_terminal_xml(FILE *fp, HCFGrammar *g, int indent)
{
  int len;
  size_t s;
  if (g->nts->used < 1)
		printf("error");

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
        pprint_t_xml(fp, g, a);
      }
    }
  }
}
 
static void pprint_nt(FILE *f, const HCFGrammar *g, const HCFChoice *nt, int indent, int len)
{
  char line[80];
  const char *name = nonterminal_namerules(g, nt);
  /* Prints non-terminals in the correct xml format using
   * 'nonteriminal_name' function to generate printable char object
  */
  sprintf(line,"<nonterminal symbol-number=\"%d\" name = \"%s\"/>", num, name);
  num++;
  put_line(line, 3, f);
}

static void h_pprint_nonterminalrules(FILE *file, const HCFGrammar *g, int indent)
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
        pprint_nt(file, g, a, indent, len);
      }
    }
  }
}


static void rangeprintrules(FILE *f, int *range) {
	int i;
	char line[80];
	
	for (i=range[0]; i<(range[1]+1); i++) {
	  if (chartable[i]== false) {
	    sprintf(line, "<terminal symbol-number=\"%d\" token-number=\"%d\" name = \"\'%c\'\"/>", num, (unsigned int)i, i);
	    num++;
	    put_line(line, 3, f);
	    fill(i);
	  }
	}
}


static void pprint_charsetrange(FILE *f, const HCharset cs)
{
  int i;
  int range[2];

  for(i=0; i<256; i++) {
    if (charset_isset(cs, i)) {
      range[0]=i;
      // detect ranges
      if (i+2<256 && charset_isset(cs, i+1) && charset_isset(cs, i+2)) {
        for(; i<256 && charset_isset(cs, i); i++);
        i--;  // back to the last in range
        range[1]=i;
        rangeprintrules(f, range);
      }
    }
  }
}

 
static HCFChoice **pprint_stringrange(FILE *f, HCFChoice **x)
{
  for(; *x; x++) {
    if ((*x)->type != HCF_CHAR)
      break;
  }
  return x;
}

static void h_pprint_symbolrange(FILE *f, const HCFGrammar *g, const HCFChoice *x)
{
  switch(x->type) {
  case HCF_CHARSET: 
     pprint_charsetrange(f, x->charset);
     break;
  default:
		break;
  }
}
 
 
static void h_pprint_sequence_range(FILE *f, const HCFGrammar *g, const HCFSequence *seq)
{
  HCFChoice **x = seq->items;
  if (*x != NULL)
    while(*x) {
      if (x != seq->items)
          fputc(' ', f); // internal separator
      
      if ((*x)->type == HCF_CHAR) {
          x = pprint_stringrange(f, x);  //getting rid of this call kills the program
      }
      else {
          h_pprint_symbolrange(f, g, *x); //function that prints the $
          x++;
    }
  }
}

static void pprint_sequence_range(FILE *f, const HCFGrammar *g, const HCFSequence *seq)
{
  h_pprint_sequence_range(f, g, seq);
}
 
static void pprint_ntrules_range(FILE *f, const HCFGrammar *g, const HCFChoice *nt)
{
  //const char *name = nonterminal_namelhs(g, nt);
  //char line[80];
  assert(nt->type == HCF_CHOICE);
  HCFSequence **p = nt->seq;
  if (*p == NULL)
      printf("error");          // shouldn't happen

  for(; *p; p++) {
    pprint_sequence_range(f, g, *p);
  }
}

static void h_pprint_grammar_range(FILE *fp, HCFGrammar *g, int indent)
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
        pprint_ntrules_range(fp, g, a);
      }
    }
  }
}


static void h_pprint_grammar_enumerated(FILE *fp, HCFGrammar *g, int indent)
{
  
}




int pp_lalr(char *pn,HParser *hmr_parser) {
	
	//just file stuff
  /* generate the output file */
  char fn[128];
  int rule_num = 0;
  
  /* generate a file name for the output XML file */
  strcpy(fn,pn);
  strcat(fn,".xml");
  printf("[Generating %s...",fn);
  fflush(stdout);
	
  /* open the XML file for writing -- fp now points to the file */
  FILE *fp;
  if((fp=fopen(fn,"w"))==NULL) {
    printf("[unable to open %s]\n",fn);
    return -1;
  }
	
  /* finess grammar to include a unique start rule */
  HAllocator *mm__ = &system_allocator;
  HCFGrammar *g = h_cfgrammar_(mm__, h_desugar_augmented(mm__, hmr_parser));
  if (g == NULL) {
    fprintf(fp, "h_cfgrammar failed\n");
    return -1;
  }
	
	//end of just file stuff
	
	
  h_pprint_structure(fp, g, 0);
	
	
	fprintf(fp, "<?xml version=\"%s\"?>\n", version);
	fprintf(fp, "\n<hammer-xml-report version=\"%s\">\n", rep_version);
	
	fprintf(fp, "<!--");
	fprintf(fp, "\nORIGINAL RULE TABLE\n");
	h_pprint_ruletable(fp, g, 0);
    expansion_print(fp);
	
	fprintf(fp, "\nORIGINAL STATE MACHINE\n");
	h_pprint_lrtable(fp,g,hmr_parser->backend_data,0);
	
	
	fprintf(fp, "-->\n");
	
	
	init();
	//printing xml rules
	put_line("<grammar>", 1, fp);
	put_line("<rules>", 2, fp);
	h_pprint_grammar_xml(fp, g, 0, rule_num);
	//printing xml grammar
	h_pprint_range(fp);
	h_pprint_enumerated(fp);
	
	
	/*int i;
	 for (i=0; i<enumerate[0]->rhslen; e++) {
       fprintf(f, "%s", enumerate[e]->rhs[i]);
    }
	
	
	fprintf(fp, "\n\n%s ", enumerate[e]->rhs[i]); */
	
	put_line("</rules>", 2, fp);
	put_line("<terminals>", 2, fp);
    h_pprint_terminal_xml(fp, g, 0);
	h_pprint_grammar_range(fp, g, 0);
	h_pprint_grammar_enumerated(fp, g, 0);
	put_line("</terminals>", 0, fp); //changed from 2, idk why it's messed up
	//printing non-terminals
	put_line("<nonterminals>", 2, fp);
	h_pprint_nonterminalrules(fp, g, 0);
	put_line("</nonterminals>", 2, fp);
	put_line("</grammar>", 1, fp);
	    int e;
    for (e=0; e<rule_numstruct; e++) {
    fprintf(fp, "Rule number %d:\n", e);
    fprintf(fp, "lhs is %s ", ruletable[e]->lhs);
    fprintf(fp, "rhs position %d is %s\n", 0, ruletable[e]->rhs[0]);
    fprintf(fp, "rhs position 1 is %s\n", ruletable[e]->rhs[1]);
    fprintf(fp, "rhs position 2 is %s\n", ruletable[e]->rhs[2]);
    fprintf(fp, "rhs position 3 is %s\n", ruletable[e]->rhs[3]);
    }
	
  //fprintf(fp,"\n\n\n======= state automaton=========\n");
  h_pprint_state(fp,g,hmr_parser->backend_data,0);
	fprintf(fp, "  </automaton>\n</hammer-xml-report>\n\n");

	
  /* close the XML file */
  fclose(fp);
  printf("]\n");
	
  return 0;
}