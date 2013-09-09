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

#define _GNU_SOURCE

#include <search.h>
#include <stdarg.h>

#include "hash/hash.h"
#include "class.h"

static
int
hashCtor(void * _this, va_list * params)
{
	return 0;
}

static
inline
void
tDelete(const void * node, const int depth)
{
	delete(node);
}

static
void
hashDtor(void * _this)
{
	Hash this = _this;

	treeDestroy(&this->root, tDelete);
}

INIT_IFACE(Class, hashCtor, hashDtor, NULL);
CREATE_CLASS(Hash, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
