/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2012  Georg Hopp
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "session.h"
#include "class.h"

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
	this->id       = sdbm((unsigned char *)uname, nuname) ^ this->livetime;

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
