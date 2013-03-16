/**
 * \file
 * Abstraction layer above BSD sockets. Capsules and simplifies connect
 * accept and listen.
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

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <arpa/inet.h>  // for in_port_t

#include "class.h"
#include "logger.h"

CLASS(Sock) {
	Logger             log;
	in_port_t          port;
	struct sockaddr_in addr;
	int                handle;
};

void socketConnect(Sock this, const char * addr, char (*)[16]);
void socketListen(Sock this, int backlog);
Sock socketAccept(Sock this, char (*remoteAddr)[16]);

#endif // __SOCKET_H__

// vim: set ts=4 sw=4:

