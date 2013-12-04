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
#include <stdio.h>

#include "trbase.h"
#include "http/response.h"
#include "http/interface/http_intro.h"


static
int
httpResponseCtor(void * _this, va_list * params)
{
	HttpResponse this = _this;
	char * reason;

	TR_PARENTCALL(_this, TR_Class, ctor, params);

	this->status = va_arg(* params, unsigned int);
	reason       = va_arg(* params, char *);

	this->reason = TR_calloc(1, strlen(reason)+1);
	strcpy(this->reason, reason);

	return 0;
}

static
void
httpResponseDtor(void * _this)
{
	HttpResponse this = _this;

	TR_MEM_FREE(this->reason);

	TR_PARENTCALL(_this, TR_Class, dtor);
} 

static
size_t
sizeGet(void * _this)
{
	HttpResponse this = _this;
	size_t       size = 0;

	size += strlen(((HttpMessage)this)->version) + 1;
	size += 3 + 1;	// for status
	size += strlen(this->reason) + 2;

	return size;
}

static
char *
toString(void * _this, char * string)
{
	HttpResponse this   = _this;

	strcpy(string, ((HttpMessage)this)->version);
	string   += strlen(string);
	*string++ = ' ';

	snprintf(string, 4, "%d", this->status);
	string   += strlen(string);
	*string++ = ' ';

	strcpy(string, this->reason);
	string   += strlen(string);
	*string++ = '\r';
	*string++ = '\n';

	return string;
}

TR_INIT_IFACE(TR_Class, httpResponseCtor, httpResponseDtor, NULL);
TR_INIT_IFACE(HttpIntro, sizeGet, toString);
TR_CREATE_CLASS(
		HttpResponse,
		HttpMessage,
		TR_IF(TR_Class),
		TR_IF(HttpIntro));

// vim: set ts=4 sw=4:
