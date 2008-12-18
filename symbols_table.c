#include "symbols_table.h"
#include "hashtable.h"
#include "hashtable_type.h"
#include "tools/math_tools.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

struct symbol_table *symbol_table_init(struct symbol_table *st)
{
        st->hashtable = (struct hashtable *) malloc(sizeof(struct hashtable));
        hashtable_init(st->hashtable,SYMBOL_TABLE_SIZE);
        return st;
}

int symbol_table_add(struct symbol_table *st, char *name, __u16 value)
{
        static int no = 1;
        struct value *val;
        
        val = (struct value *) malloc(sizeof(struct value));
        val->name = (char *) malloc(my_strlen(name) + 1);
        memcpy(val->name,name,my_strlen(name) + 1);
        val->value = value;
        val->flags = 0;

        hashtable_add(st->hashtable,my_itoa(no,10),val);
        return no++;
}

struct symbol_table *symbol_table_delete(struct symbol_table *st)
{
        hashtable_delete(st->hashtable);
        free(st->hashtable);
        return st;
}

int st_get_next_key(struct symbol_table *st)
{
        static int key = 1;
        return (key++ % st->hashtable->count);
}

char *st_get_name(struct symbol_table *st, int key)
{
        struct value *val;

        hashtable_lookup(st->hashtable,my_itoa(key,10),&val);

        return val->name;
}

/* return 0 if no name matches at the table */
int st_get_number(struct symbol_table *st, char *name)
{
        int end,i;
        struct value *val;

        end = 0;
        i = st->hashtable->count;
        while ((i) && (!end))
        {
                hashtable_lookup(st->hashtable,my_itoa(i,10),&val);
                if (!strcmp(val->name,name))
                {
                        end = i;
                }
                i--;
        }
        return end;
}

int st_set_declared(struct symbol_table *st, int key)
{
        struct value *val;
        
        hashtable_lookup(st->hashtable,my_itoa(key,10),&val);

        val->flags = val->flags|FLAG_DECLARED;
        return 0;
}

int st_set_used(struct symbol_table *st, int key)
{
        struct value *val;
        
        hashtable_lookup(st->hashtable,my_itoa(key,10),&val);
        val->flags = val->flags|FLAG_USED;

        return 0;
}

int st_is_declared(struct symbol_table *st, int key)
{
        struct value *val;

        hashtable_lookup(st->hashtable,my_itoa(key,10),&val);

        return (val->flags&FLAG_DECLARED);
}

int st_is_used(struct symbol_table *st, int key)
{
        struct value *val;

        hashtable_lookup(st->hashtable,my_itoa(key,10),&val);

        return val->flags&FLAG_USED;
}


        
/*
int main(void)
{
        struct hashtable t;
        struct value test,test2;
        
        hashtable_init(&t,4);

        test.flags = FLAG_DECLARED|FLAG_USED;
        test.value = 32;
        test.type = 55;
        
        printf("T type: %hd value: %d flags:%#x\n",test.type,test.value,test.flags);
        hashtable_add(&t,"prout",&test);
        hashtable_add(&t,"proute",&test);
        hashtable_add(&t,"proutee",&test);
        
        if (hashtable_lookup(&t,"prout",&test2))
                printf("FOUND type: %hd value: %d flags:%#x\n",test2.type,test2.value,test2.flags);
        else
                printf("NOT FOUND\n");  

        hashtable_delete(&t);
        
        if (hashtable_lookup(&t,"prout",&test2))
                printf("FOUND type: %hd value: %d flags:%#x\n",test2.type,test2.value,test2.flags);
        else
                printf("NOT FOUND\n");  

        return 0;
}
*/
/*
int main(void)
{
        struct symbol_table st;
        symbol_table_init(&st);
        symbol_table_add(&st,"prout",12);
        symbol_table_add(&st,"prout2",6);
        int i = symbol_table_add(&st,"prout3",4);
        symbol_table_add(&st,"prout4",2);
        symbol_table_add(&st,"prout5",2);
        printf("NAME:%s\n",st_get_name(&st,i));
        printf("NUMBER:%d:%d:\n",st_get_number(&st,"proute"),i);
        
        printf("USED : %d\n",st_is_used(&st,st_get_number(&st,"prout2")));
        st_set_used(&st,st_get_number(&st,"prout2"));
        printf("USED : %d\n",st_is_used(&st,st_get_number(&st,"prout2")));
        symbol_table_delete(&st);
        return 0;
}
*/
