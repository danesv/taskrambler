#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "session.h"
#include "class.h"
#include "interface/class.h"

#include "utils/hash.h"
#include "utils/memory.h"


static
int
sessionCtor(void * _this, va_list * params)
{
	Session this  = _this;
	char * uname  = va_arg(* params, char *);
	size_t nuname = va_arg(* params, size_t);

	this->livetime = time(NULL) + SESSION_LIVETIME;
	this->id       = sdbm((unsigned char *)uname, nuname) & this->livetime;

	this->username = malloc(nuname + 1);
	this->username[nuname] = 0;
	memcpy(this->username, uname, nuname);

	return 0;
}

static
void
sessionDtor(void * _this)
{
	Session this = _this;

	FREE(this->username);
}

INIT_IFACE(Class, sessionCtor, sessionDtor, NULL);
CREATE_CLASS(Session, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
