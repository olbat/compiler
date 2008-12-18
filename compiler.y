%{
	#include <stdio.h>
	#include "abstract_tree.h"
	#include "debug.h"
	#include "generator.h"
	int yylex(void);
	void yyerror(char *msg);
%}

%start program
%token LP RP LCB RCB LSB RSB SEMICOLON IF THEN ELSE WHILE DO FOR TO LET IN END COMMA PLUS MINUS MULTIPLY DIVIDE GE LE DIFFERENT GT LT AND OR AFFECT COLON EQUALS VAR FUNCTION INT REAL STRING INTVAL STRINGVAL NEG
%token <string> ID

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
	enum at_typeid r_typeid; 
	struct at_vardec *r_vardec;
	struct at_dec *r_dec;
	struct at_decs *r_decs;
	struct at_exp *r_exp;
	int base;
	char *string;
	unsigned int line;
}

%type <base> expitersemic expitersemicP expitercomma expitercomma1 expitercommaP lvalue  tabdec fundec tyfields tyfieldsiter tyfieldsiterP
%type <r_typeid> typeid
%type <r_vardec> vardec
%type <r_dec> dec 
%type <r_decs> decs
%type <r_exp> exp program

%%
			
program 	: exp
		{
			if ($1)
			{
				printf("abstract tree: ");
				print_exp($1);
				printf("\n\n");
				printf("---generating code---\n");
				generate($1);
				printf("\n---code generated---\n");
			}
		}
exp		: LP expitersemic RP  {}
		| INTVAL {}
		| STRINGVAL {}
		| MINUS exp %prec NEG {}
		| exp PLUS exp {}
		| exp MINUS exp {}
		| exp MULTIPLY exp {}
		| exp DIVIDE  exp {}
		| exp EQUALS exp {}
		| exp GE  exp {}
		| exp LE  exp {}
		| exp DIFFERENT  exp {}
		| exp GT  exp {}
		| exp LT  exp {}
		| exp AND  exp {}
		| exp OR exp {}
		| lvalue {}
		| lvalue AFFECT exp {}
		| IF exp THEN exp  {}
		| IF exp THEN exp ELSE exp {}
		| WHILE exp DO exp {}
		| FOR ID AFFECT exp TO exp DO exp {}
		| LET decs IN expitersemic END 
		{
			struct at_let *l;
			l = (__typeof__(l)) malloc(sizeof(l));
			l->decs = $2;
			
			$$ = AT_DEFINE_CHOICE(struct at_exp *,AT_ENUM_EXP_LET,l,
				let);
		}
		| ID LP expitercomma RP {}
		;
expitersemic	: exp expitersemicP {}
		| {}
		; 
expitersemicP	: SEMICOLON exp expitersemicP {}
		| {}
		;
expitercomma	: exp expitercommaP {}
		| {}
		; 
expitercomma1	: exp expitercommaP {}
		;
expitercommaP	: COMMA exp expitercommaP {}
		| {}
		;
lvalue		: ID {}
		| ID LSB exp RSB {}
		;
decs		: dec decs
		{
			struct at_decs *s;
			s = (__typeof__(s)) malloc(sizeof(s));
			s->dec = $1;
			$$ = s;
		}
		| {}
		;
dec		: vardec
		{
			struct at_dec *s;
			s = AT_DEFINE_CHOICE(__typeof__(s),AT_ENUM_DEC_VARDEC,
				$1, vardec);
			$$ = s;
		}
		| tabdec {}
		| fundec {} 
		;
vardec		: VAR ID AFFECT exp 
		{
			struct at_vardec *s;
			s = AT_DEFINE_CHOICE(__typeof__(s),AT_ENUM_VARDEC_TYPE,
				0,typeid);
			s->exp = $4;
			s->idname = $2;
			$$ = s;
			
		}
		| VAR ID COLON typeid AFFECT exp
		{
			struct at_vardec *s;
			s = AT_DEFINE_CHOICE(__typeof__(s),AT_ENUM_VARDEC_TYPE,
				$4,typeid);
			s->exp = $6;
			s->idname = $2;
			$$ = s;
		}
		;
fundec		: FUNCTION ID LP tyfields RP EQUALS exp  {}
		| FUNCTION ID LP tyfields RP COLON typeid EQUALS exp {} 
		;
tabdec		: VAR ID COLON typeid LSB ID RSB {}
		| VAR ID COLON typeid LSB ID RSB AFFECT LCB expitercomma1 RCB {}
		;
tyfields	: tyfieldsiter {} 
		| {}
		;
tyfieldsiter	: ID COLON typeid tyfieldsiterP {}
		;
tyfieldsiterP	: COMMA ID COLON typeid tyfieldsiterP {}
		| {}
		;
typeid		: INT { $$ = AT_TID_INT; }
		| REAL { $$ = AT_TID_REAL; }
		| STRING { $$ = AT_TID_STRING; }
		;

%%

void
yyerror(char *msg)
{
	fprintf(stderr,"%d: error: %s\n",yylval,msg);
}

int
main(void)
{
	yylval.line = 1;
	yyparse();
	return 0;
}
