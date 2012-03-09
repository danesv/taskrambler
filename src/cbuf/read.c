/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
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

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "cbuf.h"
#include "stream.h"


ssize_t
cbufRead(Cbuf this, Stream st)
{
	ssize_t rrsize = 0;
	size_t  rsize  = cbufGetFree(this);

	if (0 == rsize) {
		errno = ECBUFOVFL;
		return -1;
	}

	rrsize = streamRead(st, cbufGetWrite(this), rsize);

	switch (rrsize) {
		case  0:
			rrsize = -2;
			// DROP THROUGH

		case -1:
			break;

		default:
			cbufIncWrite(this, rrsize);
			break;
	}

	return rrsize;
}

// vim: set ts=4 sw=4:
