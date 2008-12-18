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
	AT_ENUM_EXP_LET
	/* >>> TODO: prout */
};

struct at_exp
{
	enum at_enum_exp e;
	union
	{
		struct at_let *let;
		/* >>> TODO: prout */
	} u;
};


/* let node*/

struct at_let
{
	struct at_decs *decs;
	/* >>> TODO: prout */
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
		enum at_typeid type;
		/* >>>TODO: prout */
	} u;

	char *idname;
};


/* decs node*/

struct at_decs
{
	struct at_dec *dec;
};


/* dec node*/

enum at_enum_dec
{
	AT_ENUM_DEC_VARDEC,
	AT_ENUM_DEC_TABDEC,
	AT_ENUM_DEC_FUNDEC
};

struct at_dec
{
	enum at_enum_dec e;
	union 
	{
		struct at_vardec *vardec;
		/* >>> TODO: prout */
	} u;
};


/* vardec node*/

enum at_enum_vardec
{
	AT_ENUM_VARDEC_NOTYPE,
	AT_ENUM_VARDEC_TYPE
};

struct at_vardec
{
	enum at_enum_vardec e;
	union
	{
		enum at_typeid typeid;
	} u;

	char *idname;
	struct at_exp *exp;
};


/* typeid node*/
enum at_typeid
{
	AT_TID_INT,
	AT_TID_REAL,
	AT_TID_STRING
};
	
#endif
