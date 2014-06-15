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

#include "trbase.h"
#include "trdata.h"
#include "http/header.h"
#include "http/parser.h"
#include "http/message.h"
#include "http/request.h"

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
			current->body  = TR_malloc(current->nbody);
		}
		current->dbody = 0;
	}

	if (0 == strncasecmp("cookie", name, nname-1)) {
		HttpRequest request = (HttpRequest)this->current;
		char *  pair = value;
		ssize_t togo = lend - value;

		while(NULL != pair && 0 < togo) {
			char * key    = pair;
			char * eqsign;
			char * val;
			size_t nval;

			for (; *key == ' ' && key < lend; key++, togo--);
			eqsign = memchr(key, '=', togo);

			if (NULL == eqsign) {
				break;
			}

			togo -= (eqsign - key);
			pair  = memchr(eqsign, ';', togo);

			if (NULL == pair) {
				pair = (char *)lend;
			}

			nval = pair-eqsign-1;
			val  = (0 != nval)? eqsign+1 : NULL;

			TR_hashAdd(request->cookies,
					TR_new(TR_HashValue, key, eqsign-key, val, nval));

			pair++;
			togo -= (pair - eqsign);
		}
	}

	TR_hashAdd(current->header,
			TR_new(HttpHeader, name, nname, value, lend - value));
}

// vim: set ts=4 sw=4:
