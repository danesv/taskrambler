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

#include <string.h>

#include "http/message.h"
#include "utils/http.h"

int
httpMessageHasValidVersion(HttpMessage this)
{
	int major;
	int minor;

	if (! isHttpVersion(this->version, strlen(this->version)))
		return 0;

	if (0 > httpMessageGetVersion(this, &major, &minor))
		return 0;

	if (1 != major)
		return 0;

	if (0 > minor || 1 < minor)
		return 0;

	return 1;
}

// vim: set ts=4 sw=4:
