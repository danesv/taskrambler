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

#include <sys/types.h>

#include "asset.h"
#include "hash.h"
#include "utils/hash.h"

static
inline
int
hashDeleteComp(const void * a, const void * b)
{
	unsigned long hash_a = hashableGetHash((void*)a);

	if (hash_a < *(const unsigned long*)b) {
		return -1;
	}
	
	if (hash_a > *(const unsigned long*)b) {
		return 1;
	}

	return 0;
}

void *
hashDelete(Hash this, const char * search, size_t nsearch)
{
	unsigned long   hash   = sdbm((const unsigned char *)search, nsearch);
	void          * found = NULL;

	found = treeDelete(&(this->root), &hash, hashDeleteComp);

	return found;
}

void *
hashDeleteByVal(Hash this, unsigned long hash)
{
	void * found = treeDelete(&(this->root), &hash, hashDeleteComp);

	return found;
}

// vim: set ts=4 sw=4:
