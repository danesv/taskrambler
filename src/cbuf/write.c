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

#include "cbuf.h"
#include "stream.h"


ssize_t
cbufWrite(Cbuf this, Stream st)
{
	ssize_t wwsize = 0;
	size_t  wsize  = this->bused;

	if (0 == wsize) return 0;

	wwsize = streamWrite(st, cbufGetRead(this), wsize);

	switch (wwsize) {
		case -1:
			break;

		default:
			cbufIncRead(this, wwsize);
			break;
	}

	return wwsize;
}

// vim: set ts=4 sw=4:
