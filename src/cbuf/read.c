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

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "trio.h"
#include "cbuf.h"


ssize_t
cbufRead(Cbuf this, TR_Stream st)
{
	size_t  rsize = cbufGetFree(this);
	ssize_t rrsize;

	if (0 == rsize) {
		errno = ECBUFOVFL;
		return -1;
	}

	rrsize = TR_streamRead(st, cbufGetWrite(this), rsize);

	if (0 < rrsize) {
		cbufIncWrite(this, rrsize);
	}

	return rrsize;
}

// vim: set ts=4 sw=4:
