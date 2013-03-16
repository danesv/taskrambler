/**
 * \file
 * HTTP header class. One Instance represents one header line of
 * an HTTP message.
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

#ifndef __HTTP_HEADER_H__
#define __HTTP_HEADER_H__

#include <sys/types.h>

#include "class.h"

#define N_VALUES	128

CLASS(HttpHeader) {
	unsigned long hash;
	char *        name;
	char *        value[N_VALUES];
	size_t        nname;			//!< len of name without \0
	size_t        nvalue[N_VALUES];	//!< len of value without \0
	size_t        cvalue;			//!< count of values up to N_VALUE
	size_t        size;				//!< full size of this header
};

size_t     httpHeaderToString(HttpHeader, char *);

#endif // __HTTP_HEADER_H__

// vim: set ts=4 sw=4:
