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

#include "server.h"
#include "logger.h"
#include "stream.h"

void serverCloseConn(Server, unsigned int);

ssize_t
serverWrite(Server this, unsigned int i)
{
	int     fd = (this->fds)[i].fd;
	ssize_t remaining;

	if (NULL == (this->conns)[fd].worker) {
		loggerLog(
				this->logger,
				LOGGER_INFO,
				"initialization error: NULL worker");
		return -1;
	}

	remaining = streamWriterWrite(
			(this->conns)[fd].worker,
			(this->conns)[fd].stream);

	switch(remaining) {
		case -1:
			serverCloseConn(this, i);
			break;

		case 0:
			(this->fds)[i].events &= ~POLLOUT;
			break;

		default:
			break;
	}

	return remaining;
}

// vim: set ts=4 sw=4:
