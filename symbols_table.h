#ifndef _SYMBOLS_TABLE_H
#define _SYMBOLS_TABLE_H

enum st_enum_type
{
	ST_ENUM_TYPE_VAR,
	ST_ENUM_TYPE_FUNC,
	ST_ENUM_TYPE_TAB
};

struct st_entry
{
	char *name;
	enum st_enum_type type;
	struct st_entry *next;
};

struct st_node
{
	struct st_entry *entries;
	int num;
	struct st_node *parent;
	struct st_node *childs;
	struct st_node *siblings;
};


struct st_node *st_node_init(struct st_node *parent);
void st_node_free(struct st_node *node);
struct st_node *st_node_add_entry(struct st_node *node, struct st_entry *entry);
void st_entries_free(struct st_entry *entry);
struct st_entry *st_entry_init(char *name, enum st_enum_type type);
struct st_entry *st_node_lookup_entry(struct st_node *node, char *name);

#endif
