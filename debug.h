#ifndef _DEBUG_H
#define _DEBUG_H

#include "abstract_tree.h"

void print_exp(struct at_exp *exp);
void print_let(struct at_let *let);
void print_decs(struct at_decs *decs);
void print_dec(struct at_dec *dec);
void print_vardec(struct at_vardec *vardec);
void print_typeid(enum at_enum_typeid idtype);

#endif
