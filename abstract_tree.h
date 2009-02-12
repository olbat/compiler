#ifndef _ABSTRACT_TREE_H
#define _ABSTRACT_TREE_H

#include <stdlib.h>

#define AT_DEFINE_CHOICE(ST,E,U,UN) \
({ \
	ST _s; \
	_s = (__typeof__(_s)) malloc(sizeof(_s)); \
	_s->e = E; \
	_s->u.UN = U; \
	_s; \
})


/* exp node*/

enum at_enum_exp
{
	AT_ENUM_EXP_EXPITERSEMIC,
	AT_ENUM_EXP_INTVAL,
	AT_ENUM_EXP_REALVAL,
	AT_ENUM_EXP_STRINGVAL,
	AT_ENUM_EXP_NEG,
	AT_ENUM_EXP_PLUS,
	AT_ENUM_EXP_MINUS,
	AT_ENUM_EXP_MULTIPLY,
	AT_ENUM_EXP_DIVIDE,
	AT_ENUM_EXP_EQUALS,
	AT_ENUM_EXP_GE,
	AT_ENUM_EXP_LE,
	AT_ENUM_EXP_DIFFERENT,
	AT_ENUM_EXP_GT,
	AT_ENUM_EXP_LT,
	AT_ENUM_EXP_AND,
	AT_ENUM_EXP_OR,
	AT_ENUM_EXP_LVALUE,
	AT_ENUM_EXP_AFFECT,
	AT_ENUM_EXP_IF,
	AT_ENUM_EXP_IFELSE,
	AT_ENUM_EXP_WHILE,
	AT_ENUM_EXP_FOR,
	AT_ENUM_EXP_LET,
	AT_ENUM_EXP_EXPITERID
};

struct at_exp
{
	enum at_enum_exp e;
	union
	{
		struct at_expitersemic *expitersemic;
		struct at_intval *intval;
		struct at_realval *realval;
		struct at_stringval *stringval;
		struct at_neg *neg;
		struct at_plus *plus;
		struct at_minus *minus;
		struct at_multiply *multiply;
		struct at_divide *divide;
		struct at_equals *equals;
		struct at_ge *ge;
		struct at_le *le;
		struct at_different *different;
		struct at_gt *gt;
		struct at_lt *lt;
		struct at_and *and;
		struct at_or *or;
		struct at_lvalue *lvalue;
		struct at_affect *affect;
		struct at_if *ift;
		struct at_ifelse *ifelse;
		struct at_while *whiled;
		struct at_for *ford;
		struct at_let *let;
		struct at_expiterid *expiterid;
	} u;
};

struct at_expitersemic
{
	struct at_exp *exp;
	struct at_expitersemic *next;
};

struct at_expitercomma
{
	struct at_exp *exp;
	struct at_expitercomma *next;
};

struct at_expiterid
{
	char *idname;
	struct at_expitercomma *iter;
};

struct at_intval
{
	int val;
};

struct at_realval
{
	float val;
};

struct at_stringval
{
	char *val;
};

struct at_neg
{
	struct at_exp *exp;	
};

struct at_plus
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_minus
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_multiply
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_divide
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_equals
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_ge
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_le
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_different
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_gt
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_lt
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_and
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_or
{
	struct at_exp *expl;
	struct at_exp *expr;
};

struct at_affect
{
	struct at_lvalue *lvalue;
	struct at_exp *exp;
};

struct at_if
{
	struct at_exp *cond;
	struct at_exp *exp;
};

struct at_ifelse
{
	struct at_exp *cond;
	struct at_exp *expthen;
	struct at_exp *expelse;
};

struct at_while
{
	struct at_exp *cond;
	struct at_exp *exp;
};

struct at_for
{
	char *idname;
	struct at_exp *init;
	struct at_exp *end;
	struct at_exp *exp;
};

/* let node*/

struct at_let
{
	struct at_decs *decs;
	struct at_expitersemic *expitersemic;
};

/* typeid node*/
enum at_enum_typeid
{
	AT_TID_INT,
	AT_TID_REAL,
	AT_TID_STRING
};


/* lvalue node*/

enum at_enum_lvalue
{
	AT_ENUM_LVALUE_VAR,
	AT_ENUM_LVALUE_TAB
};
	
struct at_lvalue
{
	enum at_enum_lvalue e;
	union
	{
		struct at_exp *exp;
	} u;
	char *idname;
};


/* decs node*/

struct at_decs
{
	struct at_dec *dec;
	struct at_decs *next;
};

/* vardec node*/

enum at_enum_vardec
{
	AT_ENUM_VARDEC_NOTYPE,
	AT_ENUM_VARDEC_TYPE
};

struct at_vardec
{
	char *idname;
	struct at_exp *exp;

	enum at_enum_vardec e;
	union
	{
		enum at_enum_typeid idtype;
	} u;
};

/*
enum at_enum_tyfields
{
	AT_ENUM_TYFIELDS_INT,
	AT_ENUM_TYFIELDS_REAL,
	AT_ENUM_TYFIELDS_STRING
};
*/
struct at_tyfields
{
	/*
	enum at_enum_tyfields e;
	union
	{
		int valint;
		float valreal;
		char *varstring;
	} u;
	*/
	enum at_enum_typeid idtype;
	char *idname;
	struct at_tyfields *next;
};

enum at_enum_fundec
{
	AT_ENUM_FUNDEC_FUNC,
	AT_ENUM_FUNDEC_PROC
};

struct at_fundec
{
	char *idname;
	struct at_tyfields *tyfields;
	struct at_exp *exp;
	enum at_enum_fundec e;
	union
	{
		enum at_enum_typeid idtype;
	} u;
};

enum at_enum_tabdec
{
	AT_ENUM_TABDEC_NORMAL,
	AT_ENUM_TABDEC_AFFECT
};

struct at_tabdec
{
	char *idname;
	int size;
	enum at_enum_typeid idtype;
	enum at_enum_tabdec e;
	union
	{
		struct at_expitercomma *expitercomma;
	} u;
};

/* dec node*/

enum at_enum_dec
{
	AT_ENUM_DEC_VARDEC,
	AT_ENUM_DEC_FUNDEC,
	AT_ENUM_DEC_TABDEC
};

struct at_dec
{
	enum at_enum_dec e;
	union 
	{
		struct at_vardec *vardec;
		struct at_fundec *fundec;
		struct at_tabdec *tabdec;
	} u;
};
	
#endif
