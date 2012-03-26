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
#include <string.h>
#include <sys/types.h>

#include "http/parser.h"
#include "http/request.h"
#include "hash.h"
#include "class.h"

void
httpParserRequestVars(HttpParser this)
{
	HttpRequest request = (HttpRequest)this->current;
	char *      delim   = strchr(request->uri, '?');

	if (NULL == delim) {
		delim = request->uri + strlen(request->uri);
	}

	request->path = malloc(delim - request->uri + 1);
	request->path[delim - request->uri] = 0;
	memcpy(request->path, request->uri, delim - request->uri);

	while(NULL != delim && 0 != *delim) {
		char * key    = delim + 1;
		char * eqsign = strchr(key, '=');
		char * value;
		size_t nvalue;

		if (NULL == eqsign) {
			return;
		}

		delim  = strchr(eqsign, '&');

		if (NULL == delim) {
			delim = key + strlen(key);
		}

		nvalue = delim-eqsign-1;
		value  = (0 != nvalue)? eqsign+1 : NULL;

		hashAdd(request->get,
				new(HashValue, key, eqsign-key, value, nvalue));
	}
}

// vim: set ts=4 sw=4:
