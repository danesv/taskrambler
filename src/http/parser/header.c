/**
 * \file
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
#include <sys/types.h>

#include "class.h"
#include "interface/class.h"
#include "http/header.h"
#include "http/parser.h"
#include "http/message.h"

void
httpParserHeader(
		HttpParser   this,
		const char * line,
		const char * lend)
{
	const char * name    = line;
	char *       value   = memchr(line, ':', lend - line);
	size_t       nname   = (value++) - name;
	HttpMessage  current = this->current;

	if (NULL == value) {
		return;
	}

	for (; *value == ' ' && value < lend; value++);

	if (value == lend) {
		return;
	}

	if (0 == strncasecmp("content-length", name, nname-1)) {
		current->nbody = strtoul(value, NULL, 10);
		if (0 < this->current->nbody) {
			current->body  = malloc(current->nbody);
		}
		current->dbody = 0;
	}

	httpHeaderAdd(
			&(current->header),
			new(HttpHeader, name, nname, value, lend - value));
}

// vim: set ts=4 sw=4:
