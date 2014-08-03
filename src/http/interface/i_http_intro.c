/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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

#include "trbase.h"
#include "http/interface/http_intro.h"

TR_CREATE_INTERFACE(HttpIntro, 2);

size_t
httpIntroSizeGet(void * object)
{
	size_t ret;

	TR_RETCALL(object, HttpIntro, sizeGet, ret);

	return ret;
}

char *
httpIntroToString(void * object, char * string)
{
	char * ret;

	TR_RETCALL(object, HttpIntro, toString, ret, string);

	return ret;
}

// vim: set ts=4 sw=4:
