/**
 * \file
 * My own class implementation for C. It combines a data structure
 * with a set of dynamically linked methods defined by an interface. A
 * dynamically linked method will be called via a selector method which in
 * turn gets the implementation stored in the class.
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

#ifndef __CLASS_CLASS_H__
#define __CLASS_CLASS_H__

#include <stdarg.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

#include "class/interface.h"

#ifndef _ISOC99_SOURCE
#define _ISOC99_SOURCE
#endif

#define CLASS_MAGIC    0xFEFE

#define CLASS(name)                      \
	struct c_##name;                     \
	typedef struct c_##name * name;      \
	extern struct class * const _##name; \
	struct c_##name

#define EXTENDS(parent) \
	const char _[sizeof(struct c_##parent)]

#define _NULL	NULL
#define CREATE_CLASS(name,_parent,...)              \
	static struct class c_##name;                   \
	static class_ptr _classInit##name##_(void) {    \
		c_##name.parent = _##_parent;               \
		c_##name.init   = NULL;                     \
		return &c_##name;                           \
	}                                               \
	static struct class c_##name = {                \
		CLASS_MAGIC,                                \
		NULL,                                       \
		sizeof(struct c_##name),                    \
		_classInit##name##_,                        \
		INIT_IFACE_IMPL(__VA_ARGS__)                \
	}; struct class * const _##name = &c_##name;	\
	struct c_##name##_object { void * class; struct c_##name data; }


/**
 * create a static instance of a class.
 * \todo
 * this macro requires to close the initializer
 * with an extra curly brancket. This is not nice...find a 
 * way to prevent this.
 */
#define INSTANCE(class, name)             \
	struct c_##class##_object _##name;    \
	class name = &(_##name.data);         \
	struct c_##class##_object _##name = { \
		&c_##class, 

#define INIT_CLASS(class)			((class)->init? (class)->init() : (class))
#define GET_CLASS(object)			(INIT_CLASS(*(class_ptr *)((void*)(object) - sizeof(void*))))
#define IFACE_GET(class,iface)		(interfaceGet(&((class)->impl),(iface)))
#define HAS_PARENT(class)			(NULL != ((class)->parent) &&  INIT_CLASS((class)->parent))

#define IS_OBJECT(obj)				((GET_CLASS((obj)))->magic == CLASS_MAGIC)
#define INSTANCE_OF(class,obj)		((GET_CLASS((obj))) == _##class)

/**
 * \todo actually i use gcc feature ## for variadoc... think about
 * a way to make this standard.
 */
#define _CALL(_class,_iface,method,...)                                         \
	do {                                                                        \
		class_ptr class = _class;                                               \
		iface = (struct i_##_iface *)IFACE_GET(class, &i_##_iface);             \
		while ((NULL == iface || NULL == iface->method) && HAS_PARENT(class)) { \
			class = class->parent;                                              \
			iface = (struct i_##_iface *)IFACE_GET(class, &i_##_iface);         \
		}                                                                       \
		assert(NULL != iface->method);                                          \
	} while(0)

#define CALL(object,_iface,method,...)                           \
	do {                                                         \
		struct i_##_iface * iface;                               \
		_CALL(GET_CLASS(object), _iface, method, ##__VA_ARGS__); \
		iface->method(object, ##__VA_ARGS__);                    \
	} while(0)

#define RETCALL(object,_iface,method,ret,...)                    \
	do {                                                         \
		struct i_##_iface * iface;                               \
		_CALL(GET_CLASS(object), _iface, method, ##__VA_ARGS__); \
		ret = iface->method(object, ##__VA_ARGS__);              \
	} while(0)

#define PARENTCALL(object,_iface,method,...)                    \
	do {                                                        \
		struct i_##_iface * iface;                              \
		class_ptr pc_class = GET_CLASS((object));               \
		assert(HAS_PARENT(pc_class));                           \
		_CALL(pc_class->parent, _iface, method, ##__VA_ARGS__); \
		iface->method(object, ##__VA_ARGS__);                   \
	} while(0)


struct class;
typedef struct class * class_ptr;
typedef class_ptr (* fptr_classInit)(void);
struct class {
	const int         magic;
	class_ptr         parent;
	size_t            object_size;
	fptr_classInit    init;
	struct iface_impl impl;
};

#endif // __CLASS_CLASS_H__

// vim: set ts=4 sw=4:
