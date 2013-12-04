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
#include <stdlib.h>
#include <unistd.h>

#include "socket.h"
#include "logger.h"
#include "trbase.h"

static
int
socketCtor(void * _this, va_list * params)
{
	Sock this = _this;
	int reUse   = 1;     //! \todo make this configurable
	int port;

	this->log  = va_arg(* params, Logger);
	port       = va_arg(* params, int);

	//! if port is -1 do not initialize the socket. (Used with accept)
	if (-1 == port) {
		return 0;
	} else {
		this->port = port;
	}

	//! Create socket for incoming connections
	if (-1 == (this->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
		loggerLog(this->log, LOGGER_CRIT,
				"error opening socket: %s - service terminated",
				strerror(errno));
		return -1;
	}

	//! Make the socket REUSE a TIME_WAIT socket
	setsockopt(this->handle, SOL_SOCKET, SO_REUSEADDR, &reUse, sizeof(reUse));

	return 0;
}

static
void
socketDtor(void * _this)
{
	Sock this = _this;

	if (STDERR_FILENO < this->handle) {
		shutdown(this->handle, SHUT_RDWR);
		close(this->handle);
	}
}

TR_INIT_IFACE(TR_Class, socketCtor, socketDtor, NULL);
TR_CREATE_CLASS(Sock, NULL, TR_IF(TR_Class));

// vim: set ts=4 sw=4:
