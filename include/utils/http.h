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

#ifndef __UTILS_HTTP_H__
#define __UTILS_HTTP_H__

#include <time.h>
#include <sys/types.h>

#include "http/message.h"

size_t      rfc1123Gmt(char *, size_t, const time_t *);
size_t      rfc1123GmtNow(char *, size_t);
char        isHttpVersion(const char *, size_t);
HttpMessage httpGetMessage(
		const char *, size_t,
		const char *, size_t,
		const char *, size_t);

#endif // __UTILS_HTTP_H__

// vim: set ts=4 sw=4:
