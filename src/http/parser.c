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
#include <sys/types.h>
#include <stdarg.h>

#include "class.h"
#include "stream.h"

#include "http/parser.h"
#include "queue.h"
#include "http/request.h"
#include "http/response.h"
#include "cbuf.h"

#include "utils/memory.h"


static
int
httpParserCtor(void * _this, va_list * params)
{
	HttpParser this = _this;

	this->buffer = va_arg(* params, Cbuf);

	if (NULL == this->buffer) {
		return -1;
	}

	this->queue = new(Queue);

	return 0;
}

static
void
httpParserDtor(void * _this)
{
	HttpParser this = _this;

	delete(this->queue);

	if (TRUE == this->ourLock)
		cbufRelease(this->buffer);

	FREE(this->incomplete);
	delete(this->current);
} 

INIT_IFACE(Class, httpParserCtor, httpParserDtor, NULL);
INIT_IFACE(StreamReader, httpParserParse);
CREATE_CLASS(HttpParser, NULL, IFACE(Class), IFACE(StreamReader));

// vim: set ts=4 sw=4:
