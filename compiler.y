%{
/* #define _COMPILER_DEBUG */
	#include <stdio.h>
	#include <string.h>
	#include "abstract_tree.h"
	#include "symbols_table.h"
	#include "errors.h"
	#include "debug.h"
	#include "generator.h"

	extern int err_error;
	
	struct st_node *curblock, *initblock;

	int yylex(void);
	void yyerror(char *msg);
%}

%start program
%token LP RP LCB RCB LSB RSB SEMICOLON IF THEN ELSE WHILE DO FOR TO LET IN END COMMA PLUS MINUS MULTIPLY DIVIDE GE LE DIFFERENT GT LT AND OR AFFECT COLON EQUALS VAR FUNCTION INT REAL STRING NEG
%token <intval> INTVAL
%token <realval> REALVAL
%token <string> ID STRINGVAL

%nonassoc DO
%nonassoc THEN
%nonassoc ELSE
%nonassoc AFFECT
%nonassoc EQUALS DIFFERENT GT LT GE LE 
%left AND OR
%left MINUS PLUS
%left MULTIPLY DIVIDE
%left NEG

%union { 
	struct at_exp *r_exp;
	struct at_expitersemic *r_expitersemic;
	struct at_expitercomma *r_expitercomma;
	struct at_decs *r_decs;
	struct at_dec *r_dec;
	struct at_lvalue *r_lvalue;
	struct at_vardec *r_vardec;
	struct at_fundec *r_fundec;
	struct at_tabdec *r_tabdec;
	struct at_tyfields *r_tyfields;
	enum at_enum_typeid r_typeid; 

	int intval;
	float realval;
	char *string;
	unsigned int line;
}

%type <r_exp> exp program 
%type <r_expitersemic> expitersemic expitersemicP 
%type <r_expitercomma> expitercomma expitercommaP expitercomma1
%type <r_dec> dec 
%type <r_decs> decs decsdecs
%type <r_lvalue> lvalue
%type <r_vardec> vardec
%type <r_tyfields> tyfields tyfieldsiter tyfieldsiterP
%type <r_fundec> fundec
%type <r_tabdec> tabdec
%type <r_typeid> typeid
%type <string> idfunc

%%
			
program 	: exp
		{
			if ($1)
			{
#ifdef _COMPILER_DEBUG
				printf("abstract tree: ");
				print_at_exp($1);
				printf("\n\n");
				printf("symbols table: ");
				print_st_node(initblock);
				printf("\n\n");
#endif
				if (err_error)
				{
					fprintf(stderr,
						"Error, can't generate code\n");
				}
				else
				{
#ifdef _COMPILER_DEBUG
					printf("---generating code---\n");
#endif
					generate($1);
#ifdef _COMPILER_DEBUG
					printf("\n---code generated---\n");
#endif
				}
			}
		}
exp		: LP expitersemic RP
		{
			struct at_expitersemic *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_EXPITERSEMIC, $2, expitersemic);
		}
		| INTVAL
		{
			struct at_intval *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->val = $1;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_INTVAL, e, intval);
		}
		| REALVAL
		{
			struct at_realval *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->val = $1;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_REALVAL, e, realval);
		}
		| STRINGVAL
		{
			int len;
			struct at_stringval *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->val = $1;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_STRINGVAL, e, stringval);
		}
		| MINUS exp %prec NEG
		{
			struct at_neg *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->exp = $2;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_NEG,
				e, neg);
		}
		| exp PLUS exp
		{
			struct at_plus *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_PLUS,
				e, plus);
		}
		| exp MINUS exp
		{
			struct at_minus *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_MINUS,
				e, minus);
		}
		| exp MULTIPLY exp
		{
			struct at_multiply *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_MULTIPLY, e, multiply);
		}
		| exp DIVIDE  exp
		{
			struct at_divide *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_DIVIDE, e, divide);
		}
		| exp EQUALS exp
		{
			struct at_equals *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_EQUALS, e, equals);
		}
		| exp GE  exp
		{
			struct at_ge *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_GE,
				e, ge);
		}
		| exp LE  exp
		{
			struct at_le *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_LE,
				e, le);
		}
		| exp DIFFERENT  exp
		{
			struct at_different *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_DIFFERENT, e, different);
		}
		| exp GT  exp
		{
			struct at_gt *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *, AT_ENUM_EXP_GT,
				e, gt);
		}
		| exp LT  exp
		{
			struct at_lt *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_LT,
				e, lt);
		}
		| exp AND  exp
		{
			struct at_and *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_AND,
				e, and);
		}
		| exp OR exp
		{
			struct at_or *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->expl = $1;
			e->expr = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_OR,
				e, or);
		}
		| lvalue
		{
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_LVALUE, $1, lvalue);
			if (!st_node_lookup_entry(curblock,$1->idname))
				err_msg_error(ERR_ENUM_ERROR_UNDECLARED,yylval.line,$1->idname);
		}
		| lvalue AFFECT exp
		{
			struct at_affect *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->lvalue = $1;
			e->exp = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_AFFECT, e, affect);
			if (!st_node_lookup_entry(curblock,$1->idname))
				err_msg_error(ERR_ENUM_ERROR_UNDECLARED,yylval.line,$1->idname);
		}
		| IF exp THEN exp 
		{
			struct at_if *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->cond = $2;
			e->exp = $4;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_IF,
				e, ift);
		}
		| IF exp THEN exp ELSE exp
		{
			struct at_ifelse *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->cond = $2;
			e->expthen = $4;
			e->expelse = $6;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_IFELSE, e, ifelse);
		}
		| WHILE exp DO exp
		{
			struct at_while *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->cond = $2;
			e->exp = $4;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_WHILE,
				e, whiled);
		}
		| FOR ID AFFECT exp TO exp DO exp
		{
			struct at_for *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->idname = $2;
			e->init = $4;
			e->end = $6;
			e->exp = $8;
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_FOR,
				e, ford);
			if (!st_node_lookup_entry(curblock,$2))
				err_msg_error(ERR_ENUM_ERROR_UNDECLARED,yylval.line,$2);
		}
		| LET decsdecs IN expitersemic END 
		{
			struct at_let *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->decs = $2;
			e->expitersemic = $4;

			__typeof__(curblock) tmp;
			tmp = curblock->parent;
                        if (curblock->parent)
                                curblock->parent = curblock->parent->parent;
			curblock = tmp;

			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_LET,e,
				let);
		}
		| ID LP expitercomma RP
		{
			struct at_expiterid *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->idname = $1;
			e->iter = $3;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,
				AT_ENUM_EXP_EXPITERID, e, expiterid);

			if (!st_node_lookup_entry(curblock,$1))
				err_msg_error(ERR_ENUM_ERROR_UNDECLARED,yylval.line,$1);
		}
		;
expitersemic	: exp expitersemicP
		{
			struct at_expitersemic *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->exp = $1;
			e->next = $2;
			$$ = e;
		}
		|
		{
			$$ = 0;
		}
		; 
expitersemicP	: SEMICOLON exp expitersemicP
		{
			struct at_expitersemic *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->exp = $2;
			e->next = $3;
			$$ = e;
		}
		|
		{
			$$ = 0;
		}
		;
expitercomma	: exp expitercommaP
		{
			struct at_expitercomma *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->exp = $1;
			e->next = $2;
			$$ = e;
		}
		|
		{
			$$ = 0;
		}
		; 
expitercomma1	: exp expitercommaP
		{
			struct at_expitercomma *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->exp = $1;
			e->next = $2;
			$$ = e;
		}
		;
expitercommaP	: COMMA exp expitercommaP
		{
			struct at_expitercomma *e;
			e = (__typeof__(e)) malloc(sizeof(e));
			e->exp = $2;
			e->next = $3;
			$$ = e;
		}
		|
		{
			$$ = 0;
		}
		;
lvalue		: ID
		{
			struct at_lvalue *l;
			l = (__typeof__(l)) malloc(sizeof(l));
			l->idname = $1;
			l->e = AT_ENUM_LVALUE_VAR;
			$$ = l;
		}
		| ID LSB exp RSB
		{
			struct at_lvalue *l;
			l = AT_DEFINE_CHOICE(__typeof__(l),AT_ENUM_LVALUE_TAB,
				$3, exp);
			l->idname = $1;
			$$ = l;
		}
		;
decsdecs	: decs
		{
			curblock = st_node_init(curblock);
			$$ = $1;
		}
decs		: dec decs
		{
			struct at_decs *s;
			s = (__typeof__(s)) malloc(sizeof(s));
			s->dec = $1;
			s->next = $2;
			$$ = s;
		}
		|
		{
			$$ = 0;
		}
		;
dec		: vardec
		{
			st_node_add_entry(curblock,
				st_entry_init(($1)->idname,ST_ENUM_TYPE_VAR));
			$$ = AT_DEFINE_CHOICE(struct at_dec *,
				AT_ENUM_DEC_VARDEC, $1, vardec);
		}
		| fundec
		{
			$$ = AT_DEFINE_CHOICE(struct at_dec *,
				AT_ENUM_DEC_FUNDEC, $1, fundec);
			/* print_fundec(($$)->u.fundec); */
		} 
		| tabdec
		{
			st_node_add_entry(curblock,
				st_entry_init(($1)->idname,ST_ENUM_TYPE_TAB));
			$$ = AT_DEFINE_CHOICE(struct at_dec *,
				AT_ENUM_DEC_TABDEC, $1, tabdec);
		}
		;
vardec		: VAR ID AFFECT exp 
		{
			struct at_vardec *v;
			v = (__typeof__(v)) malloc(sizeof(v));	
			v->e = AT_ENUM_VARDEC_NOTYPE;
			v->idname = $2;
			v->exp = $4;
			$$ = v;
		}
		| VAR ID COLON typeid AFFECT exp
		{
			struct at_vardec *v;
			v = AT_DEFINE_CHOICE(__typeof__(v),AT_ENUM_VARDEC_TYPE,
				$4, idtype);
			v->idname = $2;
			v->exp = $6;
			$$ = v;
		}
		;
/* 
 * Used for recursive declared functions
 * Without this, there is an undeclared function error
 */
idfunc		: ID
		{
			$$ = $1;

			st_node_add_entry(curblock,
				st_entry_init($$,ST_ENUM_TYPE_FUNC));
		}
fundec		: FUNCTION idfunc LP tyfields RP EQUALS exp
		{
			struct at_fundec *f;
			f = (__typeof__(f)) malloc(sizeof(f));	
			f->e = AT_ENUM_FUNDEC_PROC;
			f->idname = $2;
			f->tyfields = $4;
			f->exp = $7;
			$$ = f;

			__typeof__(curblock) tmp;
			tmp = curblock->parent;
                        if (curblock->parent)
                                curblock->parent = curblock->parent->parent;
			curblock = tmp;
		}
		| FUNCTION idfunc LP tyfields RP COLON typeid EQUALS exp
		{
			struct at_fundec *f;
			f = AT_DEFINE_CHOICE(__typeof__(f),AT_ENUM_FUNDEC_FUNC,
				$7, idtype);
			f->idname = $2;
			f->tyfields = $4;
			f->exp = $9;
			$$ = f;

			__typeof__(curblock) tmp;
			tmp = curblock->parent;
                        if (curblock->parent)
                                curblock->parent = curblock->parent->parent;
			curblock = tmp;
		} 
		;
tabdec		: VAR ID COLON typeid LSB ID RSB
		{
			struct at_tabdec *t;
			t = (__typeof__(t)) malloc(sizeof(t));
			t->idname = $2;
			t->size = atoi($6);
			t->idtype = $4;
			$$ = t;
		}
		| VAR ID COLON typeid LSB ID RSB AFFECT LCB expitercomma1 RCB
		{
			struct at_tabdec *t;
			t = AT_DEFINE_CHOICE(__typeof__(t),
				AT_ENUM_TABDEC_AFFECT, $10, expitercomma);
			t->idname = $2;
			t->size = atoi($6);
			t->idtype = $4;
			$$ = t;
		}
		;
tyfields	: tyfieldsiter
		{
			$$ = $1;
		} 
		|
		{
			$$ = 0;
		}
		;
tyfieldsiter	: ID COLON typeid tyfieldsiterP
		{
			struct at_tyfields *t;
			t = (__typeof__(t)) malloc(sizeof(t));
			t->idname = $1;
			t->idtype = $3;
			t->next = $4;

			st_node_add_entry(curblock,
				st_entry_init(t->idname,ST_ENUM_TYPE_VAR));

			$$ = t;
		}
		;
tyfieldsiterP	: COMMA ID COLON typeid tyfieldsiterP
		{
			struct at_tyfields *t;
			t = (__typeof__(t)) malloc(sizeof(t));
			t->idname = $2;
			t->idtype = $4;
			t->next = $5;

			st_node_add_entry(curblock,
				st_entry_init(t->idname,ST_ENUM_TYPE_VAR));

			$$ = t;
		}
		|
		{
			curblock = st_node_init(curblock);
			$$ = 0;
		}
		;
typeid		: INT { $$ = AT_TID_INT; }
		| REAL { $$ = AT_TID_REAL; }
		| STRING { $$ = AT_TID_STRING; }
		;

%%

void
yyerror(char *msg)
{
	fprintf(stderr,"%d: error: %s\n",yylval.line,msg);
}

int
main(void)
{
	yylval.line = 1;
	curblock = st_node_init(0);
	initblock = curblock;

	st_node_add_entry(curblock, st_entry_init("printi",ST_ENUM_TYPE_FUNC));
	st_node_add_entry(curblock, st_entry_init("printr",ST_ENUM_TYPE_FUNC));
	st_node_add_entry(curblock, st_entry_init("prints",ST_ENUM_TYPE_FUNC));

	yyparse();
	/* st_node_free(curblock); */
	return 0;
}
