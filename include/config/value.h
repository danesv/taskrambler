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

#ifndef __CONFIG_VALUE_H__
#define __CONFIG_VALUE_H__

#include <sys/types.h>

#include "hash.h"
#include "class.h"

#define MAX_CONFIG_LINE		256

#define CONFSTRA(val)		((val)->value).string, (val)->nvalue


typedef enum e_ConfigValueType {
	CONFIG_VALUE_NUMERIC = 0,
	CONFIG_VALUE_STRING  = 1
} ConfigValueType;

CLASS(ConfigValue) {
	union {
		char      * string;
		long long   number;
	} value;

	size_t          nvalue;
	ConfigValueType type;

	unsigned long   hash;
};

#endif // __CONFIG_VALUE_H__

// vim: set ts=4 sw=4:
