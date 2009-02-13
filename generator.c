#include "generator.h"
#include "abstract_tree.h"
#include "basic.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int fd = 0;
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
	fd = open("resultat.c",O_CREAT | O_WRONLY | O_TRUNC);

	WRITES(fd,"#include <stdio.h>\n");
	WRITES(fd,"void printi(int i) { printf(\"%d\\n\",i); }\n");
	WRITES(fd,"void printr(float f) { printf(\"%f\\n\",f); }\n");
	WRITES(fd,"void prints(char *s) { printf(\"%s\\n\",s); }\n");
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
	case AT_ENUM_EXP_EXPITERSEMIC:
		generate_expitersemic(exp->u.expitersemic);
		break;
	case AT_ENUM_EXP_INTVAL:
		generate_intval(exp->u.intval);
		break;
	case AT_ENUM_EXP_REALVAL:
		generate_realval(exp->u.realval);
		break;
	case AT_ENUM_EXP_STRINGVAL:
		generate_stringval(exp->u.stringval);
		break;
	case AT_ENUM_EXP_NEG:
		generate_neg(exp->u.neg);
		break;
	case AT_ENUM_EXP_PLUS:
		generate_plus(exp->u.plus);
		break;
	case AT_ENUM_EXP_MINUS:
		generate_minus(exp->u.minus);
		break;
	case AT_ENUM_EXP_MULTIPLY:
		generate_multiply(exp->u.multiply);
		break;
	case AT_ENUM_EXP_DIVIDE:
		generate_divide(exp->u.divide);
		break;
	case AT_ENUM_EXP_EQUALS:
		generate_equals(exp->u.equals);
		break;
	case AT_ENUM_EXP_GE:
		generate_ge(exp->u.ge);
		break;
	case AT_ENUM_EXP_LE:
		generate_le(exp->u.le);
		break;
	case AT_ENUM_EXP_DIFFERENT:
		generate_different(exp->u.different);
		break;
	case AT_ENUM_EXP_GT:
		generate_gt(exp->u.gt);
		break;
	case AT_ENUM_EXP_LT:
		generate_lt(exp->u.lt);
		break;
	case AT_ENUM_EXP_AND:
		generate_and(exp->u.and);
		break;
	case AT_ENUM_EXP_OR:
		generate_or(exp->u.or);
		break;
	case AT_ENUM_EXP_LVALUE:
		generate_lvalue(exp->u.lvalue);
		break;
	case AT_ENUM_EXP_AFFECT:
		generate_affect(exp->u.affect);
		break;
	case AT_ENUM_EXP_IF:
		generate_ift(exp->u.ift);
		break;
	case AT_ENUM_EXP_IFELSE:
		generate_ifelse(exp->u.ifelse);
		break;
	case AT_ENUM_EXP_WHILE:
		generate_whiled(exp->u.whiled);
		break;
	case AT_ENUM_EXP_FOR:
		generate_ford(exp->u.ford);
		break;
	case AT_ENUM_EXP_LET:
		generate_let(exp->u.let);
		break;
	case AT_ENUM_EXP_EXPITERID:
		generate_expiterid(exp->u.expiterid);
		break;
	default:
		break;
	}
}

void generate_expitersemic(struct at_expitersemic *expitersemic)
{
	while (expitersemic)
	{
		generate_exp(expitersemic->exp);
		WRITES(fd,";");
		generate_new_line();
		expitersemic = expitersemic->next;
	}
}

void generate_intval(struct at_intval *intval)
{
	char buff[64];
	int len;
	len = snprintf(buff,sizeof(buff),"%d",intval->val);
	write(fd,buff,len);
	/*
	fprintf((FILE *)fd,"%d",intval->val);
	*/
}

void generate_realval(struct at_realval *realval)
{
	char buff[64];
	int len;
	len = snprintf(buff,sizeof(buff),"%f",realval->val);
	write(fd,buff,len);
}

void generate_stringval(struct at_stringval *stringval)
{
	WRITES(fd,"\"");
	write(fd,stringval->val,strlen(stringval->val));
}

void generate_neg(struct at_neg *neg)
{
	WRITES(fd,"-(");
	generate_exp(neg->exp);
	WRITES(fd,")");
}

void generate_plus(struct at_plus *plus)
{
	GEN_BINARY_EXP(plus," + ");
}

void generate_minus(struct at_minus *minus)
{
	GEN_BINARY_EXP(minus," - ");
}

void generate_multiply(struct at_multiply *multiply)
{
	GEN_BINARY_EXP(multiply," * ");
}

void generate_divide(struct at_divide *divide)
{
	GEN_BINARY_EXP(divide," / ");
}

void generate_equals(struct at_equals *equals)
{
	GEN_BINARY_EXP(equals," == ");
}

void generate_ge(struct at_ge *ge)
{
	GEN_BINARY_EXP(ge," >= ");
}

void generate_le(struct at_le *le)
{
	GEN_BINARY_EXP(le," <= ");
}

void generate_different(struct at_different *different)
{
	GEN_BINARY_EXP(different," != ");
}

void generate_gt(struct at_gt *gt)
{
	GEN_BINARY_EXP(gt," > ");
}

void generate_lt(struct at_lt *lt)
{
	GEN_BINARY_EXP(lt," < ");
}

void generate_and(struct at_and *and)
{
	GEN_BINARY_EXP(and," && ");
}

void generate_or(struct at_or *or)
{
	GEN_BINARY_EXP(or," || ");
}

void generate_lvalue(struct at_lvalue *lvalue)
{
	write(fd,lvalue->idname,strlen(lvalue->idname));
	switch (lvalue->e)
	{
	case AT_ENUM_LVALUE_TAB:
		WRITES(fd,"[");
		generate_exp(lvalue->u.exp);
		WRITES(fd,"]");
		break;
	default:
		break;
	}
}

void generate_affect(struct at_affect *affect)
{
	generate_lvalue(affect->lvalue);
	WRITES(fd," = ");
	generate_exp(affect->exp);
}

void generate_ift(struct at_if *ift)
{
	WRITES(fd,"if (");
	generate_exp(ift->cond);
	WRITES(fd,")");
	generate_new_line();
	WRITES(fd,"{");
	nbimbr++;
	generate_new_line();
	generate_exp(ift->exp);
	WRITES(fd,";");
	nbimbr--;
	generate_new_line();
	WRITES(fd,"}");
	generate_new_line();
}

void generate_ifelse(struct at_ifelse *ifelse)
{
	WRITES(fd,"if (");
	generate_exp(ifelse->cond);
	WRITES(fd,")");
	generate_new_line();
	WRITES(fd,"{");
	nbimbr++;
	generate_new_line();
	generate_exp(ifelse->expthen);
	WRITES(fd,";");
	nbimbr--;
	generate_new_line();
	WRITES(fd,"}");
	generate_new_line();
	WRITES(fd,"else");
	generate_new_line();
	WRITES(fd,"{");
	nbimbr++;
	generate_new_line();
	generate_exp(ifelse->expelse);
	WRITES(fd,";");
	nbimbr--;
	generate_new_line();
	WRITES(fd,"}");
	generate_new_line();
}

void generate_whiled(struct at_while *whiled)
{
	WRITES(fd,"while (");
	generate_exp(whiled->cond);
	WRITES(fd,")");
	generate_new_line();
	WRITES(fd,"{");
	nbimbr++;
	generate_new_line();
	generate_exp(whiled->exp);
	WRITES(fd,";");
	nbimbr--;
	generate_new_line();
	WRITES(fd,"}");
	generate_new_line();
}

void generate_ford(struct at_for *ford)
{
	WRITES(fd,"for (");
	write(fd,ford->idname,strlen(ford->idname));
	WRITES(fd," = ");
	generate_exp(ford->init);
	WRITES(fd,"; ");
	generate_exp(ford->end);
	WRITES(fd,";;) ");
	generate_new_line();
	WRITES(fd,"{");
	nbimbr++;
	generate_new_line();
	generate_exp(ford->exp);
	WRITES(fd,";");
	nbimbr--;
	generate_new_line();
	WRITES(fd,"}");
	generate_new_line();
}

void generate_expitercomma(struct at_expitercomma *expitercomma)
{
	while (expitercomma->next)
	{
		generate_exp(expitercomma->exp);
		WRITES(fd,", ");
		expitercomma = expitercomma->next;
	}
	generate_exp(expitercomma->exp);
	
}

void generate_expiterid(struct at_expiterid *expiterid)
{
	write(fd,expiterid->idname,strlen(expiterid->idname));
	WRITES(fd,"(");
	generate_expitercomma(expiterid->iter);
	WRITES(fd,")");
}

void generate_let(struct at_let *let)
{
	if (!let)
		return;

	WRITES(fd,"{");
	nbimbr++;
	generate_new_line();
	generate_decs(let->decs);
	generate_expitersemic(let->expitersemic);
	nbimbr--;
	generate_new_line();
	WRITES(fd,"}");
	generate_new_line();
}

void generate_decs(struct at_decs *decs)
{
	if (!decs)
		return;
	while (decs)
	{
		generate_dec(decs->dec);
		decs = decs->next;
	}
}

void generate_dec(struct at_dec *dec)
{
	if (!dec)
		return;

	switch(dec->e)
	{
	case AT_ENUM_DEC_VARDEC:
		generate_vardec(dec->u.vardec);
		break;
	case AT_ENUM_DEC_FUNDEC:
		generate_fundec(dec->u.fundec);
		break;
	case AT_ENUM_DEC_TABDEC:
		generate_tabdec(dec->u.tabdec);
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
		break;
	default:
		generate_typeid(AT_TID_INT);
		break;
	}
	write(fd,vardec->idname,strlen(vardec->idname));
	WRITES(fd," = ");
	generate_exp(vardec->exp);
	WRITES(fd,";");
	generate_new_line();
}

void generate_fundec(struct at_fundec *fundec)
{
	if (!fundec)
		return;

	switch(fundec->e)
	{
	case AT_ENUM_FUNDEC_FUNC:
		generate_typeid(fundec->u.idtype);
		break;
	case AT_ENUM_FUNDEC_PROC:
		WRITES(fd,"void ");
		break;
	default:
		break;
	}

	write(fd,fundec->idname,strlen(fundec->idname));
	WRITES(fd,"(");
	generate_tyfields(fundec->tyfields);
	WRITES(fd,")");
	generate_new_line();
	WRITES(fd,"{");
	nbimbr++;
	generate_new_line();
	generate_exp(fundec->exp);
	nbimbr--;
	generate_new_line();
	WRITES(fd,"}");
	generate_new_line();
}

void generate_tabdec(struct at_tabdec *tabdec)
{
	char buff[64];
	int len;

	if (!tabdec)
		return;
	
	generate_typeid(tabdec->idtype);
	write(fd,tabdec->idname,strlen(tabdec->idname));
	WRITES(fd,"[");
	len = snprintf(buff,sizeof(buff),"%d",tabdec->size);
	write(fd,buff,len);
	WRITES(fd,"]");

	switch(tabdec->e)
	{
	case AT_ENUM_TABDEC_AFFECT:
		WRITES(fd," = { ");
		generate_expitercomma(tabdec->u.expitercomma);
		WRITES(fd," }");
		break;
	default:
		break;
	}
	WRITES(fd,";");
	generate_new_line();
}

void generate_tyfields(struct at_tyfields *tyfields)
{
	if (!tyfields)
		return;

	while (tyfields->next)
	{
		generate_typeid(tyfields->idtype);
		write(fd,tyfields->idname,strlen(tyfields->idname));
		WRITES(fd,",");
		tyfields = tyfields->next;
	}
	generate_typeid(tyfields->idtype);
	write(fd,tyfields->idname,strlen(tyfields->idname));
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
