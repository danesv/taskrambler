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
#include <openssl/ssl.h>

#include "class.h"
#include "interface/class.h"
#include "stream.h"


static
int
streamCtor(void * _this, va_list * params)
{
	Stream this = _this;
	this->type  = va_arg(* params, StreamHandleType);

	switch(this->type) {
		case STREAM_FD:
			(this->handle).fd = va_arg(* params, int);
			break;

		case STREAM_SSL:
			(this->handle).ssl = va_arg(* params, SSL*);
			break;

		default:
			return -1;
	}

	return 0;
}

static
void
streamDtor(void * _this)
{
}

INIT_IFACE(Class, streamCtor, streamDtor, NULL);
CREATE_CLASS(Stream, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
