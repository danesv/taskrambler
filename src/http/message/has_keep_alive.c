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

#include <string.h>
#include <sys/types.h>
#include <ctype.h>

#include "http/message.h"
#include "http/request.h"
#include "http/header.h"

#include "utils/memory.h"

#include "commons.h"
#include "hash.h"

char
httpMessageHasKeepAlive(HttpMessage message)
{
	HttpHeader  header;
	size_t      size;
	char *      value;

	header = hashGet(message->header, CSTRA("connection"));

	if (NULL == header) {
		return 0;
	}

	size  = (header->nvalue)[0];
	value = (header->value)[0];

	return (0 == strncasecmp("keep-alive", value, size))? 1 : 0;
}

// vim: set ts=4 sw=4:
