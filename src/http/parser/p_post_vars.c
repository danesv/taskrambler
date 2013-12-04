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

#include "http/parser.h"
#include "http/request.h"
#include "hash.h"
#include "trbase.h"

#include "utils/http.h"

/**
 * \todo this is very similar to other pair parsing
 * things... key1=val1<delim>key2=val2<delim>...keyn=valn
 * Generalize this!!!!
 */
void
httpParserPostVars(HttpParser this)
{
	HttpRequest request = (HttpRequest)this->current;
	char *      pair    = this->current->body;
	ssize_t     togo    = this->current->nbody;

	while(NULL != pair && 0 < togo) {
		char * key    = pair;
		char * eqsign = memchr(key, '=', togo);
		char * value;
		size_t nvalue;

		if (NULL == eqsign) {
			return;
		}

		togo -= (eqsign - key);
		pair  = memchr(eqsign, '&', togo);

		if (NULL == pair) {
			pair = &(this->current->body[this->current->nbody]);
		}

		nvalue = pair-eqsign-1;
		value  = (0 != nvalue)? eqsign+1 : NULL;
		nvalue = urldecode(value, nvalue);

		hashAdd(request->post,
				TR_new(HashValue, key, eqsign-key, value, nvalue));

		pair++;
		togo -= (pair - eqsign);
	}
}

// vim: set ts=4 sw=4:
