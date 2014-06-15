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

#include "trbase.h"

#include "http/header.h"
#include "http/message.h"
#include "http/parser.h"

size_t
httpParserBody(HttpParser this, const char * buf, size_t nbuf)
{
	size_t      len     = 0;
	HttpMessage current = this->current;

	if (current->dbody < current->nbody) {
		len = MIN(current->nbody - current->dbody, nbuf);

		memcpy(current->body, buf, len);

		current->dbody += len;
	}

	return len;
}

// vim: set ts=4 sw=4:
