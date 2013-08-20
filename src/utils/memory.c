/**
 * \file This holds all stufff related our memory managent.
 * I try the best as far as I can to reduce memory fragmentation
 * and unneccessary calls to alloc and free.
 *
 * To achive this I try an approach described here as "Quick Fit".
 * http://www.flounder.com/memory_allocation.htm
 *
 * The basic idea is to keep allocated memory segments and don't free
 * them again. Instead I will put them in a tree indexed by their size.
 * To get new memory I first have a look in the tree if there is
 * a fitting memory segment. Fitting mean, larger or exactly the size
 * I need. If there is one, use it. If not create a new one using 
 * usual malloc approach.
 * I won't split the reagions at all because most likely they will be
 * free soon again. This way I might waste some memory, so I have to
 * keep an eye on this.
 *
 * Right now I don't build an upper limit for allocation. The limit
 * still is the system memory itself.
 *
 * This is not implemented as a class because it will be used in the 
 * process of object creation.
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

#include "utils/memory.h"

void
ffree(void ** data)
{
	if (NULL != *data) {
		free(*data);
		*data = NULL;
	}
}   

// vim: set ts=4 sw=4:
