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
#include <uuid/uuid.h>

#include <trbase.h>
#include <trhash.h>
#include <trdata.h>

#include "session.h"


static
int
sessionCtor(void * _this, va_list * params)
{
	Session this = _this;
	uuid_t  uuid;

	this->livetime = time(NULL) + SESSION_LIVETIME;
	uuid_generate(uuid);
	uuid_unparse(uuid, this->id);

	this->hash = TR_sdbm((unsigned char *)this->id, 36);

	return 0;
}

static
void
sessionDtor(void * _this)
{
	Session this = _this;

	TR_delete(this->user);
}

static
unsigned long
sessionGetHash(void * _this)
{
	Session this = _this;

	return this->hash;
}

static
void
sessionHandleDouble(void * _this, void * _doub)
{
}

TR_INIT_IFACE(TR_Class, sessionCtor, sessionDtor, NULL);
TR_INIT_IFACE(TR_Hashable, sessionGetHash, sessionHandleDouble);
TR_CREATE_CLASS(Session, NULL, TR_IF(TR_Class), TR_IF(TR_Hashable));

// vim: set ts=4 sw=4:
