/* Copyright 2006, 2007 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GNU/GPL the license
 * see the COPYING file for more informations */

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "../parse_options.h"
#include "../network/protocols.h"

enum hashtable_type
{
	HSH_TYPE_ERR
};

struct hashtable_value
{
	union hashtable_union
	{
		struct hashtable_err_value *err;
	} u;
	enum hashtable_type type;
};

struct cell
{
	char *key;
	struct hashtable_value *value;
	struct cell *next;
};

struct hashtable
{
	int size;
	struct cell **cells;
};

struct hashtable *hashtable_init(int size);
unsigned int hashtable_hash(char *str);
struct cell *hashtable_lookup(struct hashtable *t, char *key, struct hashtable_value **value);
struct hashtable *hashtable_add(struct hashtable *t, char *key, struct hashtable_value *value);
void hashtable_delete(struct hashtable *t);
void hashtable_value_free(struct hashtable_value *val);

#include "../error.h"
struct hashtable_value *hashtable_err_value_init(enum err_id , enum err_state, char *,void (*func_err)(int));
void hashtable_err_value_free(struct hashtable_value *val);
struct hashtable_err_value *hashtable_err_lookup(struct hashtable *, enum err_id);
	
#endif
