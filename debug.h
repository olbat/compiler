#ifndef _DEBUG_H
#define _DEBUG_H

#include "abstract_tree.h"
#include "symbols_table.h"

void print_at_exp(struct at_exp *exp);
void print_at_let(struct at_let *let);
void print_at_decs(struct at_decs *decs);
void print_at_dec(struct at_dec *dec);
void print_at_vardec(struct at_vardec *vardec);
void print_at_fundec(struct at_fundec *fundec);
void print_at_tyfields(struct at_tyfields *tyfields);
void print_at_typeid(enum at_enum_typeid idtype);

void print_st_node(struct st_node *node);
void print_st_entries(struct st_entry *entry);

#endif
