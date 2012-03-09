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

#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "interface/class.h"
#include "http/header.h"
#include "utils/hash.h"

static
inline
int
comp(const void * _a, const void * _b)
{
	HttpHeader a = (HttpHeader)_a;
	HttpHeader b = (HttpHeader)_b;
	return (a->hash < b->hash)? -1 : (a->hash > b->hash)? 1 : 0;
}

HttpHeader
httpHeaderAdd(const HttpHeader * root, HttpHeader header)
{
	HttpHeader * _found = tsearch(header, (void **)root, comp);
	HttpHeader   found;

	if (NULL == _found) {
		return NULL;
	}

	found = *_found;
	
	if (found != header) {
		if (found->cvalue >= N_VALUES) {
			return NULL;
		}
		(found->nvalue)[found->cvalue]    = (header->nvalue)[0];
		(found->value)[(found->cvalue)++] = (header->value)[0];
		found->size += header->size;
		(header->value)[0] = NULL;
		delete(header);
	}

	return found;
}

// vim: set ts=4 sw=4:
