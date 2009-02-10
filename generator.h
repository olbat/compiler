#ifndef _GENERATOR_H
#define _GENERATOR_H

#include "abstract_tree.h"

#define GEN_INDENT "        "

void generate_new_line();
void generate(struct at_exp *exp);
void generate_exp(struct at_exp *exp);
void generate_let(struct at_let *let);
void generate_decs(struct at_decs *decs);
void generate_dec(struct at_dec *dec);
void generate_vardec(struct at_vardec *vardec);
void generate_typeid(enum at_enum_typeid idtype);

#endif
