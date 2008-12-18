#ifndef _SYMBOLS_TABLE_H
#define _SYMBOLS_TABLE_H

#define SYMBOL_TABLE_SIZE 8

#include "tools/hashtable.h"


struct symbol_table
{
        struct hashtable *hashtable;
};

struct symbol_table *symbol_table_init(struct symbol_table *st);
struct symbol_table *symbol_table_delete(struct symbol_table *st);
int symbol_table_add(struct symbol_table *st, char *name, __u16 value);
char *st_get_name(struct symbol_table *st, int key);
int st_get_next_key(struct symbol_table *st);
int st_get_number(struct symbol_table *st, char *name);
int st_set_declared(struct symbol_table *st, int key);
int st_set_used(struct symbol_table *st, int key);
int st_is_declared(struct symbol_table *st, int key);
int st_is_used(struct symbol_table *st, int key);

#endif
