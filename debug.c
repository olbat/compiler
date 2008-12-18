#include "debug.h"
#include "abstract_tree.h"

#include <stdio.h>

void print_exp(struct at_exp *exp)
{
	printf("exp(");
	switch (exp->e)
	{
	case AT_ENUM_EXP_LET:
		print_let(exp->u.let);
		break;
	default:
		break;
	}
	printf(")");
}

void print_let(struct at_let *let)
{
	printf("let(");
	print_decs(let->decs);
	/* >>> TODO: prout */
	printf(")");	
}

void print_decs(struct at_decs *decs)
{
	printf("decs(");
	print_dec(decs->dec);
	/* >>> TODO: prout */	
	printf(")");	
}

void print_dec(struct at_dec *dec)
{
	printf("dec(");
	switch(dec->e)
	{
	case AT_ENUM_EXP_LET:
		print_vardec(dec->u.vardec);
		break;
	default:
		break;
	}
	printf(")");	
}

void print_vardec(struct at_vardec *vardec)
{
	printf("vardec(");
	printf("idname:%s",vardec->idname);	
	switch(vardec->e)
	{
	case AT_ENUM_VARDEC_TYPE:
		print_typeid(vardec->u.typeid);
		break;
	default:
		break;
	}
	printf(")");	
}

void print_typeid(enum at_typeid typeid)
{
	printf("typeid(");
	switch(typeid)
	{
	case AT_TID_INT:
		printf("INT");
		break;
	case AT_TID_REAL:
		printf("REAL");
		break;
	case AT_TID_STRING:
		printf("STRING");
		break;
	default:
		break;
	}
	printf(")");	
}
