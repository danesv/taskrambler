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

#include "hash.h"
#include "interface/hashable.h"
#include "interface/class.h"

static
inline
int
hashAddComp(const void * a, const void * b)
{
	return hashableGetHash((void*)b) - hashableGetHash((void*)a);
}

void *
hashAdd(Hash this, void * operand)
{
	void * found = tsearch(operand, &(this->root), hashAddComp);

	if (NULL == found) {
		return NULL;
	}

	if (operand != *(void**)found) {
		hashableHandleDouble(*(void**)found, operand);
		delete(operand);
	}

	return *(void**)found;
}

// vim: set ts=4 sw=4:
