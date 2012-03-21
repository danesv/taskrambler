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
#include <sys/types.h>

#include "hash.h"
#include "interface/hashable.h"
#include "utils/hash.h"

static
inline
int
hashDeleteComp(const void * a, const void * b)
{
	return hashableGetHash((void*)b) - *(const unsigned long*)a;
}

void *
hashDelete(Hash this, const char * search, size_t nsearch)
{
	unsigned long hash  = sdbm((const unsigned char *)search, nsearch);
	void *        found = tfind(&hash, &(this->root), hashDeleteComp);

	return (NULL != found)? *(void**)found : NULL;
}

// vim: set ts=4 sw=4:
