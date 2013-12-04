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
#include <stdio.h>
#include <stdarg.h>

#include "trbase.h"
#include "hash/interface/hashable.h"

TR_CREATE_INTERFACE(Hashable, 2);

unsigned long
hashableGetHash(void * hashable)
{
	unsigned long ret;

	TR_RETCALL(hashable, Hashable, getHash, ret);

	return ret;
}

void
hashableHandleDouble(void * hashable, void * new_hashable)
{
	TR_CALL(hashable, Hashable, handleDouble, new_hashable);
}

// vim: set ts=4 sw=4:
