#ifndef _ERRORS_H
#define _ERRORS_H

enum err_enum_list
{
	ERR_ENUM_ERROR_UNDECLARED,
	ERR_ENUM_WARN_UNUSED,
	ERR_ENUM_WARN_UNINITIALIZED
};

struct err_list
{
	enum err_enum_list e;
	char *errmsg;
};

#endif
