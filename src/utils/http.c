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

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "http/message.h"
#include "http/request.h"
#include "http/response.h"

#include "class.h"

#include "commons.h"

char
isHttpVersion(const char * str, size_t len)
{
	if (NULL == str)
		return FALSE;

	if (8 > len)
		return FALSE;

	if (0 != memcmp("HTTP/", str, sizeof("HTTP/")-1))
		return FALSE;

	return TRUE;
}

HttpMessage
httpGetMessage(
		const char * part1, size_t len1,
		const char * part2, size_t len2,
		const char * part3, size_t len3)
{
	if (isHttpVersion(part1, len1)) {
		return new(HttpResponse,
				part1, len1,
				strtoul(part2, NULL, 10),
				part3, len3);
	}

	if (isHttpVersion(part3, len3)) {
		return new(HttpRequest,
				part1, len1,
				part2, len2,
				part3, len3);
	}

	return NULL;
}

// vim: set ts=4 sw=4:
