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

#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "interface/class.h"
#include "stream.h"

void
serverCloseConn(Server this, unsigned int i)
{
	int    fd = (this->fds)[i].fd;
	Stream st = (this->conns[(this->fds)[i].fd]).stream;

	delete((this->conns)[fd].sock);
	delete((this->conns)[fd].worker);

	if (NULL != st && STREAM_SSL == st->type) {
		SSL_shutdown((st->handle).ssl);
	}

	delete((this->conns)[fd].stream);

	memset(&(this->fds[i]), 0, sizeof(struct pollfd));
}

// vim: set ts=4 sw=4:
