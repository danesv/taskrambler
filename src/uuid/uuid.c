/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2013  Georg Hopp
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

#include <stdarg.h>
#include <string.h>

#include "class.h"
#include "uuid.h"


static
int
uuidCtor(void * _this, va_list * params)
{
	return 0;
}

static
void
uuidDtor(void * _this)
{
}

static
void
uuidClone(void * _this, void * _base)
{
	Uuid this = _this;
	Uuid base = _base;

	memcpy((this->uuid).value, (base->uuid).value, 16);
}

INIT_IFACE(Class, uuidCtor, uuidDtor, uuidClone);
CREATE_CLASS(Uuid, NULL, IFACE(Class));

INSTANCE(Uuid, uuidZero) {
	.uuid.value = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
}};

// vim: set ts=4 sw=4:
