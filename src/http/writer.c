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

#include "class.h"
#include "stream.h"

#include "queue.h"
#include "http/writer.h"

static
int
httpWriterCtor(void * _this, va_list * params)
{
	HttpWriter this = _this;

	this->buffer = va_arg(*params, Cbuf);
	this->queue  = new(Queue);

	return 0;
}

static
void
httpWriterDtor(void * _this)
{
	HttpWriter this = _this;

	delete(this->queue);

	if (TRUE == this->ourLock)
		cbufRelease(this->buffer);

	if (NULL != this->current)
		delete(this->current);
}

INIT_IFACE(Class, httpWriterCtor, httpWriterDtor, NULL);
INIT_IFACE(StreamWriter, httpWriterWrite);
CREATE_CLASS(HttpWriter, NULL, IFACE(Class), IFACE(StreamWriter));

// vim: set ts=4 sw=4:
