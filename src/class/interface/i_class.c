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

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#include "class/class.h"
#include "class/interface/class.h"

#include "utils/memory.h"


const
struct interface i_Class = {
	"class",
	3
};

void *
classNew(class_ptr class, ...)
{
	void *  object = memCalloc(1, class->object_size + sizeof(void*));
	va_list params;
	int     ret;

	* (class_ptr *)object = class;
	object += sizeof(void*);

	va_start(params, class);
	RETCALL(object, Class, ctor, ret, &params);
	va_end(params);

	if (-1 == ret) {
		classDelete(&object);
	}

	return object;
}

void
classDelete(void ** object)
{
	if (NULL != *object) {
		void * mem;

		CALL(*object, Class, dtor);

		mem = *object - sizeof(void*);
		MEM_FREE(mem);
		*object = NULL;
	}
}

void *
classClone(void * _object)
{
	class_ptr class  = GET_CLASS(_object);
	void *    object = memCalloc(1, class->object_size + sizeof(void*));

	* (class_ptr *)object = class;
	object += sizeof(void*);

#undef clone
	CALL(object, Class, clone, _object);

	return object;
}

// vim: set ts=4 sw=4:
