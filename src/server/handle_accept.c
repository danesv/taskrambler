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
#include <stdio.h>
#include <stdlib.h>

#include <openssl/ssl.h>

#include "trbase.h"
#include "http/worker.h"
#include "server.h"
#include "logger.h"
#include "stream.h"

int
serverHandleAccept(Server this, unsigned int i)
{
	char   remoteAddr[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	Sock   acc = NULL;
	Stream st;

	if (this->nfds >= this->max_fds) {
		return -1;
	}

	acc = socketAccept((0 == i)? this->sock : this->sockSSL, &remoteAddr);

	if (NULL != acc && -1 != acc->handle) {
		socketNonblock(acc);

		switch(i) {
			case 0:
				// no SSL
				st = TR_new(Stream, STREAM_FD, acc->handle);
				break;

			case 1:
				// SSL
				{
					SSL * ssl = SSL_new(this->ctx);
					SSL_set_fd(ssl, acc->handle);
					SSL_accept(ssl);
					st = TR_new(Stream, STREAM_SSL, ssl);
				}
				break;

			default:
				st = NULL;
				break;
		}

		// save the socket handle
		(this->conns)[acc->handle].sock   = acc; 

		// clone worker
		(this->conns)[acc->handle].worker = TR_clone(this->worker);
		(this->conns)[acc->handle].stream = st;

		(this->fds)[this->nfds].fd        = acc->handle;
		(this->fds)[this->nfds].events    = POLLIN;
		this->nfds++;
	} else {
		TR_delete(acc);

		switch(errno) {
			case EAGAIN|EWOULDBLOCK:
			case EINTR:
				loggerLog(this->logger,
						LOGGER_DEBUG,
						"server accept blocks");
				return -1;
				break;

			default:
				loggerLog(this->logger,
						LOGGER_DEBUG,
						"server accept error");
				return -2;
				break;
		}
	}

	if (0 == this->nfds%200) {
		loggerLog(this->logger,
				LOGGER_DEBUG, "paralel connections: %lu", this->nfds);
	}

	return acc->handle;
}

// vim: set ts=4 sw=4:
