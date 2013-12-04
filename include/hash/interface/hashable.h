/**
 * \file
 * The logger interface.
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

#ifndef __HASH_INTERFACE_HASHABLE_H__
#define __HASH_INTERFACE_HASHABLE_H__

#include "trbase.h"

typedef unsigned long (* fptr_hashableGetHash)(void *);
typedef void          (* fptr_hashableHandleDouble)(void *, void *);

TR_INTERFACE(Hashable) {
	TR_IFID;
	fptr_hashableGetHash      getHash;
	fptr_hashableHandleDouble handleDouble;
};

extern unsigned long hashableGetHash(void *);
extern void          hashableHandleDouble(void *, void *);

#endif // __HASH_INTERFACE_HASHABLE_H__

// vim: set ts=4 sw=4:
