/**
 * \file
 * Virtual (dynamically bound) methods to get the size of the http intro
 * line. As i have two kinds of http messages (response, request) which
 * need a slitly different handling when turning the data into a string.
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

#ifndef __HTTP_INTRO_H__
#define __HTTP_INTRO_H__

#include <sys/types.h>

typedef size_t (* fptr_httpIntroSizeGet)(void *);
typedef char * (* fptr_httpIntroToString)(void *, char *);

extern const struct interface i_HttpIntro;

struct i_HttpIntro {
	const struct interface * const _;
	fptr_httpIntroSizeGet    sizeGet;
	fptr_httpIntroToString   toString;
};

extern size_t httpIntroSizeGet(void *);
extern char * httpIntroToString(void *, char *);

#endif // __HTTP_INTRO_H__

// vim: set ts=4 sw=4:
