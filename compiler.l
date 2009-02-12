%{
#include "abstract_tree.h"
#include "y.tab.h"
#include <string.h>

#define ID_MAX_LEN 16

/* enlever commentaires et identificateurs trop longs */
void yyerror(char *msg);
%}

%%
\(		{ return LP; }
\)		{ return RP; }
\{		{ return LCB; }
\}		{ return RCB; }
\[		{ return LSB; }
\]		{ return RSB; }
\;		{ return SEMICOLON; }
if		{ return IF; }
then		{ return THEN; }
else		{ return ELSE; }
while		{ return WHILE; }
do		{ return DO; }
for		{ return FOR; }
to		{ return TO; }
let		{ return LET; }
in		{ return IN; }
end		{ return END; }
\,		{ return COMMA; }
\+		{ return PLUS; }
\-		{ return MINUS; }
\*		{ return MULTIPLY; }
\/		{ return DIVIDE; }
\>=		{ return GE; }
\<=		{ return LE; }
\<\>		{ return DIFFERENT; }
\>		{ return GT; }
\<		{ return LT; }
\&\&		{ return AND; }
\|\|		{ return OR; }
:=		{ return AFFECT; }
:		{ return COLON; }
=		{ return EQUALS; }
var		{ return VAR; }
function	{ return FUNCTION; }
int		{ return INT; }
real		{ return REAL; }
string		{ return STRING; }
[a-zA-Z_][a-zA-Z_0-9]* { 
			if (strlen(yytext) > ID_MAX_LEN)
				yyerror("ident too long");
			else
			{
				int len;
				len = strlen(yytext) + 1;
				yylval.string = (__typeof__(yylval.string)) 
					malloc((len * sizeof(yylval.string)));
				memcpy(yylval.string,yytext,len);
				return ID;
			}
		}
[0-9]+		{ 
			yylval.intval = atoi(yytext);
			return INTVAL;
		}
[0-9]+\.[0-9]+  {
			yylval.realval = atof(yytext);
			return REALVAL;
		}
\".*\"		{
			int len;
			len = strlen(yytext);
			yylval.string = (__typeof__(yylval.string)) 
				malloc((len * sizeof(yylval.string)));
			memcpy(yylval.string,yytext + 1,len - 1);
			yylval.string[len] = 0;
			return STRINGVAL;
		}
\/\*.*\*\/	;
[ \t]		;
[\n]		{ yylval.line++; }
.		{ fprintf(stderr,"(caractere inconnu) "); yyerror(yytext); }

%%

int 
yywrap(void)
{
	return 1;
}
