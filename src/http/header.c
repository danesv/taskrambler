/**
 * \file
 * Interface implementation for HTTP header class.
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
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

#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "interface/class.h"
#include "http/header.h"

#include "utils/hash.h"
#include "utils/memory.h"

static
int
httpHeaderCtor(void * _this, va_list * params) {
	HttpHeader this = _this;
	char * name;
	char * value;

	name            = va_arg(* params, char *);
	this->nname     = va_arg(* params, size_t);
	value           = va_arg(* params, char *);
	this->nvalue[0] = va_arg(* params, size_t);
	
	this->name              = malloc(this->nname + 1);
	this->name[this->nname] = 0;
	memcpy(this->name, name, this->nname);

	this->hash = sdbm((unsigned char *)name, this->nname);

	(this->value)[0]                    = malloc((this->nvalue)[0] + 1);
	(this->value)[0][(this->nvalue)[0]] = 0;
	memcpy((this->value)[0], value, (this->nvalue)[0]);
	this->cvalue = 1;
	this->size   = this->nname + 2 + (this->nvalue)[0] + 2;

	return 0;
}

static
void
httpHeaderDtor(void * _this)
{
	HttpHeader this = _this;
	size_t     i;

	FREE(this->name);

	for (i=0; i<this->cvalue; i++) {
		FREE(this->value[i]);
	}
}

INIT_IFACE(Class, httpHeaderCtor, httpHeaderDtor, NULL);
CREATE_CLASS(HttpHeader, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
