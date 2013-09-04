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

#include <stdio.h>

#include <search.h>
#include <sys/types.h>

#include "asset.h"
#include "hash.h"
#include "utils/hash.h"

static
inline
int
hashDeleteComp(const void * a, const void * b)
{
	if (_Asset == GET_CLASS(b)) {
		Asset data = (Asset)b;
		printf("DEBUG: search asset hash: %lu\n",
				*(const unsigned long*)a);
		printf("DEBUG: found: %lu, key: %s\n",
				data->hash, data->fname);
	}

	return hashableGetHash((void*)b) - *(const unsigned long*)a;
}

void
action(const void *nodep, const VISIT which, const int depth)
{
	void * datap = *(void **)nodep;

	if (_Asset == GET_CLASS(datap)) {
		Asset data = (Asset)datap;

		switch (which) {
			case preorder:
				break;
			case postorder:
				printf("DEBUG: %s(%lu) => %p\n", data->fname, data->hash, data);
				break;
			case endorder:
				break;
			case leaf:
				printf("DEBUG: %s(%lu) => %p\n", data->fname, data->hash, data);
				break;
		}
	}
}

void *
hashDelete(Hash this, const char * search, size_t nsearch)
{
	unsigned long    hash   = sdbm((const unsigned char *)search, nsearch);
	void          *  found = NULL;
	int              count = 0;

	twalk(this->root, action);
	while (found == NULL && count < 3) {
		found = tdelete(&hash, &(this->root), hashDeleteComp);
		if (found == NULL) {
			puts("DEBUG: !!!!! NOT FOUND !!!!!!!");
			void * found = hashGet(this, search, nsearch);
			printf("DEBUG: find results in %p\n", found);
		}
		count++;
	}
	puts("===");
	twalk(this->root, action);

	return found;
}

// vim: set ts=4 sw=4:
