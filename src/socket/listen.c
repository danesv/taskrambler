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
#include "class.h"
#include "logger.h"


void
socketListen(Sock this, int backlog)
{
    (this->addr).sin_family      = AF_INET;           // Internet address family
    (this->addr).sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
    (this->addr).sin_port        = htons(this->port); // Local port

	/**
     * Bind to the local address
	 */
    if (-1 == bind(this->handle, (struct sockaddr *) &(this->addr), sizeof(this->addr))) {
        loggerLog(this->log, LOGGER_CRIT,
                "error binding socket: %s - service terminated",
                strerror(errno));
        exit(EXIT_FAILURE);
    }

	/**
     * Mark the socket so it will listen for incoming connections
	 */
    if (-1 == listen(this->handle, backlog)) {
        loggerLog(this->log, LOGGER_CRIT,
                "error binding socket: %s - service terminated",
                strerror(errno));
        exit(EXIT_FAILURE);
    }
}

// vim: set ts=4 sw=4:
