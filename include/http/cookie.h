/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * - Copyright © 2012  Georg Hopp
 * -
 * - This program is free software: you can redistribute it and/or modify
 * - it under the terms of the GNU General Public License as published by
 * - the Free Software Foundation, either version 3 of the License, or
 * - (at your option) any later version.
 * -
 * - This program is distributed in the hope that it will be useful,
 * - but WITHOUT ANY WARRANTY; without even the implied warranty of
 * - MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * - GNU General Public License for more details.
 * -
 * - You should have received a copy of the GNU General Public License
 * - along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HTTP_COOKIE_H__
#define __HTTP_COOKIE_H__

#include <time.h>
#include <sys/types.h>

#include "class.h"

CLASS(HttpCookie) {
	unsigned long hash;

	char *        key;
	char *        value;
	char *        domain;
	char *        path;

	time_t        expires;
	time_t        max_age;

	size_t        nkey;
	size_t        nvalue;
};

char * httpCookieToString(HttpCookie);
HttpCookie httpStringToCookie(const char *);

#endif // __HTTP_COOKIE_H__

// vim: set ts=4 sw=4:
