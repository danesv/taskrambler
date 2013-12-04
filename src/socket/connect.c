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

#include <stdlib.h>     // for atoi() and exit()
#include <errno.h>      // for errno

#include "socket.h"
#include "logger.h"


void
socketConnect(Sock this, const char * addr, char (*remoteAddr)[16])
{
	inet_pton(AF_INET, addr, &((this->addr).sin_addr));
    (this->addr).sin_family = AF_INET;           // Internet address family
    (this->addr).sin_port   = htons(this->port); // Local port

	if (-1 == connect(
				this->handle,
				(struct sockaddr*) &(this->addr),
				sizeof(this->addr)))
	{
        loggerLog(this->log, LOGGER_CRIT,
                "error connection socket: %s - service terminated",
                strerror(errno));
        exit(EXIT_FAILURE);
    } else {
		strcpy(*remoteAddr, inet_ntoa((this->addr).sin_addr));

        loggerLog(this->log, LOGGER_INFO,
				"handling connection %s\n", inet_ntoa((this->addr).sin_addr));
    }
}

// vim: set ts=4 sw=4:
