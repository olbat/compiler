#ifndef _GENERATOR_H
#define _GENERATOR_H

#include "abstract_tree.h"

#define GEN_INDENT "        "

#define GEN_BINARY_EXP(ST,OPS) \
	generate_exp((ST)->expl); \
	WRITES(fd,OPS); \
	generate_exp((ST)->expr);
	

void generate_new_line();
void generate(struct at_exp *exp);
void generate_exp(struct at_exp *exp);
void generate_expitersemic(struct at_expitersemic *expitersemic);
void generate_intval(struct at_intval *intval);
void generate_realval(struct at_realval *realval);
void generate_stringval(struct at_stringval *stringval);
void generate_neg(struct at_neg *neg);
void generate_plus(struct at_plus *plus);
void generate_minus(struct at_minus *minus);
void generate_multiply(struct at_multiply *multiply);
void generate_divide(struct at_divide *divide);
void generate_equals(struct at_equals *equals);
void generate_ge(struct at_ge *ge);
void generate_le(struct at_le *le);
void generate_different(struct at_different *different);
void generate_gt(struct at_gt *gt);
void generate_lt(struct at_lt *lt);
void generate_and(struct at_and *and);
void generate_or(struct at_or *or);
void generate_lvalue(struct at_lvalue *lvalue);
void generate_affect(struct at_affect *affect);
void generate_ift(struct at_if *ift);
void generate_ifelse(struct at_ifelse *ifelse);
void generate_whiled(struct at_while *whiled);
void generate_ford(struct at_for *ford);
void generate_expitercomma(struct at_expitercomma *expitercomma);
void generate_expiterid(struct at_expiterid *expiterid);
void generate_let(struct at_let *let);
void generate_decs(struct at_decs *decs);
void generate_dec(struct at_dec *dec);
void generate_vardec(struct at_vardec *vardec);
void generate_fundec(struct at_fundec *fundec);
void generate_tabdec(struct at_tabdec *tabdec);
void generate_tyfields(struct at_tyfields *tyfields);
void generate_typeid(enum at_enum_typeid idtype);


#endif
