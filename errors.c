#include "errors.h"
#include <stdio.h>

struct err_list list[] = {
	{	ERR_ENUM_ERROR_UNDECLARED, "undeclared variable" },
	{	ERR_ENUM_WARN_UNINITIALIZED, "initialised variable" },
	{	ERR_ENUM_WARN_UNUSED, "unused variable" },
	{	0,	0	}
};

char *
err_lookup_enum(enum err_enum_list e)
{
	struct err_list *ptr;
	ptr = list;

	while (ptr->errmsg)
	{
		if (ptr->e == e)
			break;
	}

	if (ptr->errmsg)
		return ptr->errmsg;
	else
		return 0;
}

void
err_msg(
	enum err_enum_list e,
	int line,
	char *name)
{
	fprintf(stderr,"%d: error: %s %s\n",line,err_lookup_enum(e),name);
}
