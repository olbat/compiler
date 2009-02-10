#include "generator.h"
#include "abstract_tree.h"
#include "basic.h"

#include <unistd.h>
#include <string.h>

const int fd = 1;
int nbimbr = 1;

void generate_new_line()
{
	int i = nbimbr;
	WRITES(fd,"\n");
	while(i-- > 0)
		WRITES(fd,GEN_INDENT);
}

void generate(struct at_exp *exp)
{
	WRITES(fd,"int main(void)\n{");
	generate_new_line();
	generate_exp(exp);
	WRITES(fd,"\n}\n");
}

void generate_exp(struct at_exp *exp)
{
	if (!exp)
		return;
	switch (exp->e)
	{
	case AT_ENUM_EXP_LET:
		generate_let(exp->u.let);
		break;
	default:
		break;
	}
}

void generate_let(struct at_let *let)
{
	if (!let)
		return;
	generate_decs(let->decs);
	/* >>> TODO: prout */
}

void generate_decs(struct at_decs *decs)
{
	if (!decs)
		return;
	generate_dec(decs->dec);
	/* >>> TODO: prout */	
}

void generate_dec(struct at_dec *dec)
{
	if (!dec)
		return;
	switch(dec->e)
	{
	case AT_ENUM_EXP_LET:
		generate_vardec(dec->u.vardec);
		break;
	default:
		break;
	}
}

void generate_vardec(struct at_vardec *vardec)
{
	if (!vardec)
		return;
	switch(vardec->e)
	{
	case AT_ENUM_VARDEC_TYPE:
		generate_typeid(vardec->u.idtype);
		write(fd,vardec->idname,strlen(vardec->idname));
		WRITES(fd,";");
		generate_new_line();
		break;
	default:
		break;
	}
}

void generate_typeid(enum at_enum_typeid idtype)
{
	switch(idtype)
	{
	case AT_TID_INT:
		WRITES(fd,"int ");
		break;
	case AT_TID_REAL:
		WRITES(fd,"float ");
		break;
	case AT_TID_STRING:
		WRITES(fd,"char *");
		break;
	default:
		break;
	}
}
