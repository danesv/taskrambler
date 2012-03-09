/**
 * \file
 * Get a header from a tree containing headers by its name.
 * The key for the tree is the hased lowercase header identifier.
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
#include <stdlib.h>

#include "http/header.h"
#include "utils/hash.h"

static
inline
int
comp(const void * _a, const void * _b)
{
	const unsigned long * a = _a;
	HttpHeader            b = (HttpHeader)_b;
	return (*a < b->hash)? -1 : (*a > b->hash)? 1 : 0;
}

HttpHeader
httpHeaderGet(const HttpHeader * root, const char * name, size_t nname)
{
	unsigned long hash = sdbm((const unsigned char*)name, nname);

	HttpHeader * found  = tfind(&hash, (void**)root, comp);

	return (NULL != found)? *found : NULL;
}

// vim: set ts=4 sw=4:
