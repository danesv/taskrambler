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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "class.h"
#include "utils/hash.h"
#include "utils/memory.h"
#include "commons.h"

#include "hash/value.h"
#include "hash/interface/hashable.h"

static
int
hashValueCtor(void * _this, va_list * params)
{
	HashValue this = _this;
	char * key     = va_arg(* params, char*);
	void * value;
	
	this->nkey   = va_arg(* params, size_t);
	value        = va_arg(* params, void*);
	this->nvalue = va_arg(* params, size_t);

	this->key = memMalloc(this->nkey + 1);
	this->key[this->nkey] = 0;
	memcpy(this->key, key, this->nkey);

	this->hash = sdbm((unsigned char *)this->key, this->nkey);

	if (NULL != value) {
		this->value = memMalloc(this->nvalue + 1);
		((char*)this->value)[this->nvalue] = 0;
		memcpy(this->value, value, this->nvalue);
	}

	return 0;
}

static
void
hashValueDtor(void * _this)
{
	HashValue this = _this;

	MEM_FREE(this->key);
	MEM_FREE(this->value);
}

static
unsigned long
hashValueGetHash(void * _this)
{
	HashValue this = _this;

	return this->hash;
}

static
void
hashValueHandleDouble(void * _this, void * _double)
{
	HashValue this = _this;
	HashValue doub = _double;
	void * tmp_value;
	size_t tmp_nvalue;

	/**
	 * here we swap the internal data of both objects,
	 * effectively overwriting the old entry. We need not
	 * to free anything here as _double will be deleted
	 * afterwards anyway (\see hash/add.c).
	 */
	tmp_value   = this->value;
	this->value = doub->value;
	doub->value = tmp_value;

	tmp_nvalue   = this->nvalue;
	this->nvalue = doub->nvalue;
	doub->nvalue = tmp_nvalue;
}

INIT_IFACE(Class, hashValueCtor, hashValueDtor, NULL);
INIT_IFACE(Hashable, hashValueGetHash, hashValueHandleDouble);
CREATE_CLASS(HashValue, NULL, IFACE(Class), IFACE(Hashable));

// vim: set ts=4 sw=4:
