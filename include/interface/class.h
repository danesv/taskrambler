/**
 * \file
 * Interface for class handling. Defines new, delete and clone selectors
 * which in turn use the ctor, dtor and clone implementation from the
 * class implementation.
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
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

#ifndef __INTERFACE_CLASS_H__
#define __INTERFACE_CLASS_H__

#include <stdarg.h>

#include "class.h"
#include "interface.h"

typedef int  (* fptr_ctor)(void *, va_list *);
typedef void (* fptr_dtor)(void *);
typedef void (* fptr_clone)(void *, void * const);

extern const struct interface i_Class;

struct i_Class {
	const struct interface * const _;
	fptr_ctor                      ctor;
	fptr_dtor                      dtor;
	fptr_clone                     clone;
};

extern void * classNew(class_ptr, ...);
extern void   classDelete(void **);
extern void * classClone(void *);

#define new(class,...)		classNew(_##class, ##__VA_ARGS__)
#define delete(object)		classDelete((void **)&(object))
#define clone(object)		classClone((void *)(object))

#endif // __INTERFACE_CLASS_H__

// vim: set ts=4 sw=4:
