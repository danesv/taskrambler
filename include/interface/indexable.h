/**
 * \file
 * This interface provides only one function at all.
 * indexUuid will generate a uuid to the current object.
 * 
 * \todo
 * Maybe merge hashable and indexable. Thus we might get an
 * easy way to exchange the hashing mechanism used for my
 * associative arrays.
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

#ifndef __INDEXABLE_H__
#define __INDEXABLE_H__

#include "uuid.h"


typedef Uuid (* fptr_indexUuid)(void *, Uuid);

extern const struct interface i_Indexable;

struct i_Indexable {
	const struct interface * const _;
	fptr_indexUuid                 uuid;
};

extern Uuid indexUuid(void *, Uuid);

#endif // __INDEXABLE_H__

// vim: set ts=4 sw=4:
