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

#include <search.h>
#include <string.h>
#include <sys/types.h>

#include "http/message.h"
#include "http/response.h"
#include "http/header.h"
#include "interface/http_intro.h"
#include "hash.h"

static size_t size;

static
inline
void
addHeaderSize(const void * node)
{
	size += ((HttpHeader)node)->size;
}

size_t
httpMessageHeaderSizeGet(HttpMessage message)
{
	size = httpIntroSizeGet(message);

	hashEach(message->header, addHeaderSize);
	size += 2;

	return size;
}

// vim: set ts=4 sw=4:
