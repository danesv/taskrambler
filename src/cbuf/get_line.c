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

#include <string.h>

#include "cbuf.h"

char *
cbufGetLine(Cbuf this, char ** line_end)
{
	char * nl  = cbufMemchr(this, '\n');
	char * ret = NULL;

	if (NULL != nl) {
		size_t len = cbufAddrIndex(this, nl) + 1;

		*line_end = nl - 1;
		*nl       = 0;
		*(nl-1)   = ('\r' == *(nl-1))? 0 : *(nl-1);

		ret = cbufGetRead(this);
		cbufIncRead(this, len);
	}

	return ret;
}

// vim: set ts=4 sw=4:
