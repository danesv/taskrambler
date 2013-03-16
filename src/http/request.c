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

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>

#include "class.h"
#include "hash.h"
#include "http/interface/http_intro.h"

#include "http/request.h"
#include "utils/memory.h"


static
int
httpRequestCtor(void * _this, va_list * params)
{
	HttpRequest this = _this;
	char      * method, * uri;
	size_t      mlen, ulen;

	method = va_arg(* params, char *);
	mlen   = va_arg(* params, size_t);
	uri    = va_arg(* params, char *);
	ulen   = va_arg(* params, size_t);

	PARENTCALL(_this, Class, ctor, params);

	this->method       = malloc(mlen + 1);
	this->method[mlen] = 0;
	memcpy(this->method, method, mlen);

	this->uri       = malloc(ulen + 1);
	this->uri[ulen] = 0;
	memcpy(this->uri, uri, ulen);

	this->get     = new(Hash);
	this->post    = new(Hash);
	this->cookies = new(Hash);

	return 0;
}

static
void
httpRequestDtor(void * _this)
{
	HttpRequest this = _this;

	delete(this->get);
	delete(this->post);
	delete(this->cookies);

	FREE(this->uri);
	FREE(this->method);
	FREE(this->path);

	PARENTCALL(_this, Class, dtor);
} 

static
size_t
sizeGet(void * _this)
{
	HttpRequest this = _this;
	size_t      size = 0;

	size += strlen(this->method) + 1;
	size += strlen(this->uri) + 1;
	size += strlen(((HttpMessage)this)->version) + 2;

	return size;
}

static
char *
toString(void * _this, char * string)
{
	HttpRequest this = _this;

	strcpy(string, this->method);
	string    += strlen(string);
	*string++  = ' ';

	strcpy(string, this->uri);
	string    += strlen(string);
	*string++  = ' ';

	strcpy(string, ((HttpMessage)this)->version);
	string    += strlen(string);
	*string++  = '\r';
	*string++  = '\n';

	return string;
}

INIT_IFACE(Class, httpRequestCtor, httpRequestDtor, NULL);
INIT_IFACE(HttpIntro, sizeGet, toString);
CREATE_CLASS(HttpRequest,
		HttpMessage,
		IFACE(Class),
		IFACE(HttpIntro));

// vim: set ts=4 sw=4:
