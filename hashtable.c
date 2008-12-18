#include "hashtable.h"

#include <asm/types.h>
#include <malloc.h>
#include <string.h>

#include "math_tools.h"
#include "compiler.h"

struct hashtable *hashtable_init(int size)
{
	struct hashtable *end;
	end = (struct hashtable *) malloc(sizeof(struct hashtable));
	end->size = size;
	end->cells = (struct cell **) malloc(size*sizeof(struct cell *));
	memset((char *)end->cells,0,size*sizeof(struct cell *));
	return end;
}

unsigned int hashtable_hash(char *str)
{
	unsigned int end;
	end = 0;
	while (*str)
	{
		end = (*str * *str) + end;
		str++;
	}
	return end;
}

struct cell *hashtable_lookup(struct hashtable *t, char *key, struct hashtable_value **value)
{
	struct cell *end,*c;

	c = *(t->cells + (hashtable_hash(key) % t->size));
	end = 0;
	while ((c) && (!end))
	{
		if (!strcmp(c->key,key))
			end = c;
		else
			c = c->next;
	}
	if ((end) && (value))
		*value = c->value;
	return end;
}

void hashtable_delete(struct hashtable *t)
{
	int i;
	struct cell *ptr,*tmpptr;
	i = 0;
	while (i < t->size)
	{
		ptr = *(t->cells + i++);
		while (ptr)
		{
			hashtable_value_free(ptr->value);
			free(ptr->key);
			tmpptr = ptr->next;
			free(ptr);
			ptr = tmpptr;
		}
	}
	free(t->cells);
	free(t);
}

struct hashtable *hashtable_add(struct hashtable *t, char *key, struct hashtable_value *value)
{
	struct cell **table,*newcell;
	int len;

	table = (t->cells + (hashtable_hash(key) % t->size));
	newcell = (struct cell *) malloc(sizeof(struct cell));
	len = strlen(key) + 1;
	
	newcell->key = (char *) malloc(len*sizeof(char));
	memcpy(newcell->key,key,len);
	/* newcell->key[len] = 0; */
	newcell->value = value;

	newcell->next = *table;
	*table = newcell;
	
	return t;
}

void hashtable_value_free(struct hashtable_value *val)
{
	/* to be changed if more types */
	hashtable_err_value_free(val);
	/*
	switch (val->type)
	{
		case HSH_TYPE_ERR :
			hashtable_err_value_free(val);
			break;
	}
	*/
}

struct hashtable_value *hashtable_err_value_init(enum err_id id, enum err_state st, char *msg, void (*func_err)(int))
{
	struct hashtable_value *end;

	end = (struct hashtable_value *) malloc(sizeof(struct hashtable_value));
	end->u.err = (struct hashtable_err_value *) malloc(sizeof(struct hashtable_err_value));
	end->u.err->id = id;
	end->u.err->state = st;
	end->u.err->msg = msg;
	end->u.err->func = func_err;
	
	return end;
}

void hashtable_err_value_free(struct hashtable_value *val) 
{
	free(val->u.err);
	free(val);
}

struct hashtable_err_value *hashtable_err_lookup(struct hashtable *t, enum err_id id)
{
	struct hashtable_value *val;

	__extension__ char key[my_itoa_buffer_size(id,10)];
	my_itoa(id,10,key);
	if (hashtable_lookup(t,key,&val))
		return val->u.err;
	else
		return 0;
}

