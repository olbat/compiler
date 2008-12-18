/* This file is a part of groinc
 * 
 * Copyright (C) 2006, 2007 Sarzyniec Luc <olbat@xiato.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * see the COPYING file for more informations */

#include "linked_list.h"
#include <malloc.h>
#include <string.h>

struct linked_list *linked_list_init()
{
	struct linked_list *end;
	end = (struct linked_list *) malloc(sizeof(struct linked_list));
	end->next = 0;
	end->value = 0;
	return end;
}

void linked_list_free(struct linked_list *l)
{
	struct linked_list *ptr;
	ptr = l;
	while (l)
	{
		ptr = l;
		l = l->next;
		linked_list_value_free(ptr->value);
		free(ptr);
	}
}

struct linked_list_value *linked_list_add(struct linked_list *l,struct linked_list_value *val)
{
	if (l->value)
	{
		struct linked_list *ptr;
		ptr = l;
		while (l->next)
			l = l->next;
		l->next = linked_list_init(); 
		l = l->next;
	}
	l->value = val;
	return val;
}

struct linked_list_value *linked_list_err_value_init(enum err_id id, char *arg)
{
	struct linked_list_value *end;
	
	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value));
	end->type = LKD_TYPE_ERR;

	end->u.err = (struct linked_list_err_value *) malloc(sizeof(struct linked_list_err_value));
	end->u.err->id = id;
	end->u.err->arg = arg;

	return end;
}

void linked_list_err_value_free(struct linked_list_value *val)
{
	free(val->u.err);
	free(val);
}

void linked_list_value_free(struct linked_list_value *val)
{
	if (val)
	{
		switch (val->type)
		{
			case LKD_TYPE_ERR : 
				linked_list_err_value_free(val);
				break;
		}
	}
}
