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
#include <sys/types.h>
#include <stdarg.h>

#include "trbase.h"
#include "trio.h"
#include "trdata.h"
#include "http/parser.h"
#include "http/request.h"
#include "http/response.h"


static
int
httpParserCtor(void * _this, va_list * params)
{
	HttpParser this = _this;

	this->buffer = va_arg(* params, TR_Cbuf);

	if (NULL == this->buffer) {
		return -1;
	}

	this->queue = TR_new(TR_Queue);

	return 0;
}

static
void
httpParserDtor(void * _this)
{
	HttpParser this = _this;

	TR_delete(this->queue);

	if (TRUE == this->ourLock)
		TR_cbufRelease(this->buffer);

	TR_MEM_FREE(this->incomplete);
	TR_delete(this->current);
} 

TR_INIT_IFACE(TR_Class, httpParserCtor, httpParserDtor, NULL);
TR_INIT_IFACE(TR_StreamReader, httpParserParse);
TR_CREATE_CLASS(HttpParser, NULL, NULL, TR_IF(TR_Class), TR_IF(TR_StreamReader));

// vim: set ts=4 sw=4:
