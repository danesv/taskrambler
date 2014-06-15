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

#include "trbase.h"
#include "trdata.h"
#include "http/interface/http_intro.h"
#include "http/request.h"


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

	TR_PARENTCALL(_this, TR_Class, ctor, params);

	this->method       = TR_malloc(mlen + 1);
	this->method[mlen] = 0;
	memcpy(this->method, method, mlen);

	this->uri       = TR_malloc(ulen + 1);
	this->uri[ulen] = 0;
	memcpy(this->uri, uri, ulen);

	this->method_id = httpRequestGetMethodId(this);

	if (-1 == this->method_id) {
		TR_MEM_FREE(this->uri);
		TR_MEM_FREE(this->method);
		TR_MEM_FREE(this->path); /** \todo looks like path is not used at all */

		TR_PARENTCALL(_this, TR_Class, dtor);

		return -1;
	}

	this->get     = TR_new(TR_Hash);
	this->post    = TR_new(TR_Hash);
	this->cookies = TR_new(TR_Hash);

	return 0;
}

static
void
httpRequestDtor(void * _this)
{
	HttpRequest this = _this;

	TR_delete(this->get);
	TR_delete(this->post);
	TR_delete(this->cookies);

	TR_MEM_FREE(this->uri);
	TR_MEM_FREE(this->method);
	TR_MEM_FREE(this->path);

	TR_PARENTCALL(_this, TR_Class, dtor);
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

TR_INIT_IFACE(TR_Class, httpRequestCtor, httpRequestDtor, NULL);
TR_INIT_IFACE(HttpIntro, sizeGet, toString);
TR_CREATE_CLASS(HttpRequest,
		HttpMessage,
		TR_IF(TR_Class),
		TR_IF(HttpIntro));

// vim: set ts=4 sw=4:
