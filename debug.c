#include "debug.h"
#include "abstract_tree.h"
#include "symbols_table.h"

#include <stdio.h>

void print_at_exp(struct at_exp *exp)
{
	printf("exp(");
	if (!exp)
		return;
	switch (exp->e)
	{
	case AT_ENUM_EXP_LET:
		print_at_let(exp->u.let);
		break;
	default:
		break;
	}
	printf(")");
}

void print_at_let(struct at_let *let)
{
	printf("let(");
	if (!let)
		return;
	print_at_decs(let->decs);
	printf(")");	
}

void print_at_decs(struct at_decs *decs)
{
	printf("decs(");
	if (!decs)
		return;
	while (decs)
	{
		print_at_dec(decs->dec);
		decs = decs->next;
	}
	printf(")");	
}

void print_at_dec(struct at_dec *dec)
{
	printf("dec(");
	if (!dec)
		return;
	switch(dec->e)
	{
	case AT_ENUM_DEC_VARDEC:
		print_at_vardec(dec->u.vardec);
		break;
	case AT_ENUM_DEC_FUNDEC:
		print_at_fundec(dec->u.fundec);
		break;
	default:
		break;
	}
	printf(")");	
}

void print_at_vardec(struct at_vardec *vardec)
{
	printf("vardec(");
	if (!vardec)
		return;
	printf("idname:%s ",(vardec->idname ? vardec->idname : "{none}"));
	switch(vardec->e)
	{
	case AT_ENUM_VARDEC_TYPE:
		print_at_typeid(vardec->u.idtype);
		break;
	default:
		break;
	}
	printf(")");	
	print_at_exp(vardec->exp);
}

void print_at_fundec(struct at_fundec *fundec)
{
	printf("fundec(");
	if (!fundec)
		return;

	printf("idname:%s ",(fundec->idname ? fundec->idname : "{none}"));

	switch(fundec->e)
	{
	case AT_ENUM_FUNDEC_FUNC:
		print_at_typeid(fundec->u.idtype);
		break;
	default:
		break;
	}

	print_at_tyfields(fundec->tyfields);
	print_at_exp(fundec->exp);

	printf(")");	
}

void print_at_tyfields(struct at_tyfields *tyfields)
{
	printf("tyfields(");
	if (!tyfields)
		return;

	while (tyfields)
	{
		printf("idname:%s ",(tyfields->idname ? tyfields->idname 
			: "{none}"));
		print_at_typeid(tyfields->idtype);
		printf(",");
		tyfields = tyfields->next;
	}
	printf("\010");

	printf(")");	
}

void print_at_typeid(enum at_enum_typeid idtype)
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

void print_st_node(struct st_node *node)
{
	__typeof__(node->childs) ptr;

	if (!node)
		return;
	
	printf("Block[%d] ",node->num);
	print_st_entries(node->entries);

	ptr = node->siblings;
	while (ptr)
	{
		printf("|");
		print_st_node(ptr);
		ptr = ptr->siblings;
	}

	if (node->childs)
	{
		printf("{");
		print_st_node(node->childs);
		printf("}");
	}

}

void print_st_entries(struct st_entry *entry)
{
	if (!entry)
		return;

	printf("name:%s [",entry->name);
	switch (entry->type)
	{
	case ST_ENUM_TYPE_VAR:
		printf("VAR");
		break;
	case ST_ENUM_TYPE_FUNC:
		printf("FUNC");
		break;
	case ST_ENUM_TYPE_TAB:
		printf("TAB");
		break;
	default:
		break;
	}
	printf("], ");
	
	if (entry->next)
		print_st_entries(entry->next);
}

