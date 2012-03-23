/**
 * \file
 * Interface definition code. Each interface is a set of selector functions
 * as well as a data structure where the concrete implementation will be stored.
 * This structure is the intergrated in the class that implements the
 * interface.
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

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <sys/types.h>

#define MAX_IFACE	32	// ATTENTION: every iface_impl will use MAX_IFACE * sizeof(void*)

#define IFACE(name)		((const struct i_##name const*)&i_##name##_impl)
#define INIT_IFACE(name,...) \
	static const struct i_##name i_##name##_impl = {&i_##name,__VA_ARGS__}

#define NUMARGS(...)	(sizeof((const void*[]){__VA_ARGS__})/sizeof(void*))
#define INIT_IFACE_IMPL(...)	{NUMARGS(__VA_ARGS__), 0, {__VA_ARGS__}}


struct interface {
	const char * name;
	const size_t nmethods;
};
typedef const struct interface * iface_ptr;

struct iface_impl {
	const size_t nimpl;           // number of interface implementations
	char         simpl;           // implementations sorted??
	const void * impl[MAX_IFACE]; // implementations
};
typedef struct iface_impl * iface_impl_ptr;

extern iface_ptr interfaceGet(iface_impl_ptr, const iface_ptr);

#endif // __INTERFACE_H__

// vim: set ts=4 sw=4:
