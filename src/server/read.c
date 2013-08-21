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

#include <errno.h>

#include "server.h"
#include "logger.h"
#include "stream.h"

void    serverCloseConn(Server, unsigned int);

ssize_t
serverRead(Server this, unsigned int i)
{
	int     fd = (this->fds)[i].fd;
	ssize_t size;

	if (NULL == (this->conns)[fd].worker) {
		loggerLog(
				this->logger,
				LOGGER_INFO,
				"initialization error: NULL reader");
		return -1;
	}

	switch ((size = streamReaderRead(
					(this->conns)[fd].worker,
					(this->conns)[fd].stream)))
	{
		case -1: 
			/*
			 * read failure
			 */
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				/* on EGAIN just try again later. */
				break;
			}
			// DROP-THROUGH

		case -2:
			/**
			 * normal close: this must be mapped to -2 within the
			 * underlying read call.
			 *
			 * \todo make sure all pending writes will be done before
			 * close.
			 */

			/*
			 * close connection if not EAGAIN, this would also
			 * remove the filedescriptor from the poll list.
			 * Else just return indicate
			 */
			loggerLog(this->logger, LOGGER_INFO,
					"connection[%d] closed...%s",
					fd,
					inet_ntoa((((this->conns)[fd].sock)->addr).sin_addr));
			serverCloseConn(this, i);

		case 0:
			break;

		default:
			(this->fds)[i].events |= POLLOUT;
			break;
	}

	return size;
}

// vim: set ts=4 sw=4:
