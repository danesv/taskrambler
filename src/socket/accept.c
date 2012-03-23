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

#include <errno.h>      // for errno
#include <unistd.h>

#include "socket.h"
#include "class.h"
#include "interface/logger.h"

Sock
socketAccept(Sock this, char (*remoteAddr)[16])
{
    Sock         sock;   // Socket for client
    unsigned int len;    // Length of client address data structure

    // Set the size of the in-out parameter
    len = sizeof(this->addr);

	/**
	 * \todo Uhh, this is bad. we open a new socket additionally to
	 * the one we get from the accept call.
	 * i have to change the socket constructor to be able to create
	 * the data structure without creation of a socket at all.
	 * For now i simply close the socket here.... :D
	 */
	sock = new(Sock, this->log, -1);

    // Wait for a client to connect
    sock->handle = accept(this->handle, (struct sockaddr *) &(sock->addr), &len);
    if (-1 == sock->handle) {
        loggerLog(this->log, LOGGER_WARNING,
                "error accepting connection: %s", strerror(errno));
    } else {
		strcpy(*remoteAddr, inet_ntoa((sock->addr).sin_addr));

        loggerLog(this->log, LOGGER_INFO,
				"handling client %s\n", inet_ntoa((sock->addr).sin_addr));
    }

    return sock;
}

// vim: set ts=4 sw=4:
