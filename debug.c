#include "debug.h"
#include "abstract_tree.h"

#include <stdio.h>

void print_exp(struct at_exp *exp)
{
	printf("exp(");
	if (!exp)
		return;
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
	if (!let)
		return;
	print_decs(let->decs);
	printf(")");	
}

void print_decs(struct at_decs *decs)
{
	printf("decs(");
	if (!decs)
		return;
	while (decs)
	{
		print_dec(decs->dec);
		decs = decs->next;
	}
	printf(")");	
}

void print_dec(struct at_dec *dec)
{
	printf("dec(");
	if (!dec)
		return;
	switch(dec->e)
	{
	case AT_ENUM_DEC_VARDEC:
		print_vardec(dec->u.vardec);
		break;
	case AT_ENUM_DEC_FUNDEC:
		print_fundec(dec->u.fundec);
		break;
	default:
		break;
	}
	printf(")");	
}

void print_vardec(struct at_vardec *vardec)
{
	printf("vardec(");
	if (!vardec)
		return;
	printf("idname:%s ",(vardec->idname ? vardec->idname : "{none}"));
	switch(vardec->e)
	{
	case AT_ENUM_VARDEC_TYPE:
		print_typeid(vardec->u.idtype);
		break;
	default:
		break;
	}
	printf(")");	
	printf("\n");
	print_exp(vardec->exp);
}

void print_fundec(struct at_fundec *fundec)
{
	printf("fundec(");
	if (!fundec)
		return;

	printf("idname:%s ",(fundec->idname ? fundec->idname : "{none}"));

	switch(fundec->e)
	{
	case AT_ENUM_VARDEC_TYPE:
		print_typeid(fundec->u.idtype);
		break;
	default:
		break;
	}

	/* >>>TODO: fix this bug */
	print_tyfields(fundec->tyfields);
	print_exp(fundec->exp);

	printf(")");	
}

void print_tyfields(struct at_tyfields *tyfields)
{
	printf("tyfields(");
	if (!tyfields)
		return;

	while (tyfields)
	{
		print_typeid(tyfields->idtype);
		printf("idname:%s ",(tyfields->idname ? tyfields->idname 
			: "{none}"));
		printf(",");
		tyfields = tyfields->next;
	}

	printf(")");	
}

void print_typeid(enum at_enum_typeid idtype)
{
	printf("idtype(");
	switch(idtype)
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
