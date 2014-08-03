/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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
#include <stdarg.h>
#include <sys/types.h>

#include <trbase.h>
#include <trhash.h>

#include "hash.h"
#include "http/cookie.h"


static
int
httpCookieCtor(void * _this, va_list * params)
{
	HttpCookie this   = _this;
	char *     key    = va_arg(* params, char*);
	char *     value;

	this->nkey   = va_arg(* params, size_t);
	value        = va_arg(* params, char*);
	this->nvalue = va_arg(* params, size_t);

	this->key = TR_malloc(this->nkey + 1);
	this->key[this->nkey] = 0;
	memcpy(this->key, key, this->nkey);

	this->value = TR_malloc(this->nvalue + 1);
	this->value[this->nvalue] = 0;
	memcpy(this->value, value, this->nvalue);

	this->hash = TR_sdbm((unsigned char *)key, nkey);

	return 0;
}

static
void
httpCookieDtor(void * _this, va_list * params)
{
	HttpCookie this = _this;

	TR_MEM_FREE(this->key);
	TR_MEM_FREE(this->value);
	TR_MEM_FREE(this->domain);
	TR_MEM_FREE(this->path);
}

static
unsigned long
httpCookieGetHash(void * _this)
{
	HttpCookie this = _this;

	return this->hash;
}

static
void
httpCookieHandleDouble(void * _this, void * _double)
{
	HttpCookie this = _this;
	HttpCookie doub = _double;

	SWAP(char*, this->key, doub->key);
	SWAP(char*, this->value, doub->value);
	SWAP(char*, this->domain, doub->domain);
	SWAP(char*, this->path, doub->path);

	SWAP(char*, this->nkey, doub->nkey);
	SWAP(char*, this->nvalue, doub->nvalue);
}


TR_INIT_IFACE(TR_Class, httpCookieCtor, httpCookieDtor, NULL);
TR_INIT_IFACE(Hashable, httpCookieGetHash, httpCookieHandleDouble);
TR_CREATE_CLASS(HttpCookie, NULL, NULL, TR_IF(TR_Class), TR_IF(Hashable));

// vim: set ts=4 sw=4:
