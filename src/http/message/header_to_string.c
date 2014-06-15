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
#include <stdio.h>

#include "trdata.h"
#include "http/response.h"
#include "http/header.h"
#include "http/interface/http_intro.h"

static char * string;

static
inline
void
addHeaderString(const void * node)
{
	string += httpHeaderToString((HttpHeader)node, string);
}

char *
httpMessageHeaderToString(HttpMessage response, char * _string)
{
	HttpMessage message = (HttpMessage)response;

	string = httpIntroToString(response, _string);

	TR_hashEach(message->header, addHeaderString);

	*string++ = '\r';
	*string++ = '\n';

	return string;
}

// vim: set ts=4 sw=4:
