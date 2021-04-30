#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pp_lalr.h"
#include "pp_lalr_xml.h"


//WORKING: a structure which holds a single rule
typedef struct ruleinfo {
	char *lhs;
	int rhslen;
	char *rhs[10];
} rule_t;

char *currlhs;
char *currrhs[10];
int currrhslen = 0;


//WORKING: this is the array where all rules are stored
rule_t *ruletable[1000];

rule_t *currrule;

rule_t *endrule;

int rule_numstruct = 0;

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

static char str1state[]= "    ";
static char str2state[]= "      ";

//WORKING: rp must be universal so rhs char *[] can be added to as it is defined
rule_t *rp;

//WORKING: nextrule keeps track of which rule is being writing in the array ruletable
int nextrule=0;

int tester = 0;

int firstterm = 1;

int endred = 0;
int reductionmarker = 0;


void put_line(int tabs, FILE *f) {
	int i;
	for(i=0;  i<tabs; i++)
		fprintf(f, "    ");   /* put a tab, tabs times */
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
	
	for(i=0; i<nextrule; i++) {
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


//added


static void h_pprint_symbolstate(FILE *f, const HCFGrammar *g, const HCFChoice *x)
{
  switch(x->type) {
  case HCF_END:
    fprintf(f, "$end");
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




static void h_pprint_symbolstruct(FILE *f, const HCFGrammar *a, const HCFChoice *z)
{
	char *ntrhs;
	
	ntrhs = nonterminal_namerhs(a, z);
	rp->rhs[track] = get_lhs(ntrhs);
	
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
  ruletable[nextrule] = rp;
	
	
  //WORKING: now, increment nextrule to add another rule to the array
  nextrule++;
	
	
	
  for(; *p; p++) {                // print the rest below with "or" bars
    rp = make_empty_rule();
    track = 0; //new rhs definition coming
    tester = 0;
		
    //LHS
    rp->lhs = get_lhs(name);
		
    //RHS
    h_pprint_sequencestruct(f, g, *p);
		
    //WORKING: the rule has been completely defined- add to the ruletable at location nextrule
    ruletable[nextrule] = rp;
		
		
    //WORKING: now, increment nextrule to add another rule to the array
    nextrule++;
		
		
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

char* h_pprint_symbolreduction(FILE *f, const HCFGrammar *a, const HCFChoice *z)
{
	char* lhs;
	lhs = nonterminal_namelhscurr(a, z);
	return lhs;
}

void h_pprint_charred(FILE *f, uint8_t c)
{
	char *rhs;
	char *rhscopy;
	
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



void h_pprint_symbolred(FILE *f, const HCFGrammar *a, const HCFChoice *z)
{
	char *ntrhs;
	
	ntrhs = nonterminal_namerhscurr(a, z);
	currrule->rhs[track2] = get_lhs(ntrhs);
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
    currrule->rhslen = track2;
    track2 = 0;
		
    rulenum = match_rule(currrule);
		
		fprintf(f, "        <reductions>\n");
    fprintf(f,"          <reduction symbol= \"$default\" rule=\"%d\"/>\n", rulenum);
    reductionyn = 1;
    reductionmarker = 1;
		
		
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
  h_pprint_sequence(f, g, *p++);    // print first production on the same line
  fprintf(f, "\n");
  rule_numstruct ++;
  //INCREMENT SPOT TO WRITE TO
	
  for(; *p; p++) {                // print the rest below with "or" bars
		fprintf( f, "rule %d: ", rule_numstruct);
    fputs(name, f);
    fputs(" -> ", f);
    h_pprint_sequence(f, g, *p);
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






int pp_lalr(char *pn,HParser *hmr_parser) {
	
	//just file stuff
  /* generate the output file */
  char fn[128];
	
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
	
	fprintf(fp, "\nORIGINAL STATE MACHINE\n");
	h_pprint_lrtable(fp,g,hmr_parser->backend_data,0);
	
	
	fprintf(fp, "-->\n");
	
	
	pp_lalr_xml(fp, g, 0);
	
	
  //fprintf(fp,"\n\n\n======= state automaton=========\n");
  h_pprint_state(fp,g,hmr_parser->backend_data,0);
	fprintf(fp, "  </automaton>\n</hammer-xml-report>\n\n");
	
	
  /* close the XML file */
  fclose(fp);
  printf("]\n");
	
  return 0;
}
