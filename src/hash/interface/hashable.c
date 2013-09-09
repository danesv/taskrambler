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

#include "class.h"
#include "hash/interface/hashable.h"

const struct interface i_Hashable = {
	"hashable",
	2
};

unsigned long
hashableGetHash(void * hashable)
{
	unsigned long ret;

	//RETCALL(hashable, Hashable, getHash, ret);
	do {
		struct i_Hashable * iface;
		//_CALL(GET_CLASS(hashable), Hashable, getHash);
		do {
			class_ptr class = GET_CLASS(hashable);
			iface = (struct i_Hashable *)IFACE_GET(class, &i_Hashable);
			while ((NULL == iface || NULL == iface->getHash) && HAS_PARENT(class)) {
				class = class->parent;
				iface = (struct i_Hashable *)IFACE_GET(class, &i_Hashable);
			}
			assert(NULL != iface->getHash);
		} while(0);

		ret = iface->getHash(hashable);
	} while(0);


	return ret;
}

void
hashableHandleDouble(void * hashable, void * new_hashable)
{
	CALL(hashable, Hashable, handleDouble, new_hashable);
}

// vim: set ts=4 sw=4:
