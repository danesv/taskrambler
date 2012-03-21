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

#include <string.h>
#include <stdlib.h>

#include "http/message.h"

int
httpMessageGetVersion(HttpMessage this, int * major, int * minor)
{
	char * major_ptr = this->version + 5;
	char * minor_ptr = strchr(major_ptr, '.') + 1;
	char   version[] = "\0\0\0";

	if (NULL == minor_ptr ||
			((minor_ptr - major_ptr - 1) > 2) ||
			strlen(minor_ptr) > 2)
		return -1;

	memcpy(version, major_ptr, minor_ptr - major_ptr - 1);
	*major = atoi(version);

	memset(version, 0, 3);
	strcpy(version, minor_ptr);
	*minor = atoi(version);

	return ((*major)<<7)|(*minor);
}

// vim: set ts=4 sw=4:
