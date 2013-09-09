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

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "class.h"
#include "server.h"
#include "socket.h"
#include "logger.h"

#include "utils/memory.h"


void serverCloseConn(Server, unsigned int);


static
int
serverCtor(void * _this, va_list * params)
{
	Server       this = _this;
	in_port_t    port;
	unsigned int backlog;
	int          flags;

	this->max_fds = sysconf(_SC_OPEN_MAX);
	if (this->max_fds <= 10) {	// reserve 10 handles for internal use.
		/**
		 * \todo some logging would be appropriate :)
		 */
		return -1;
	}
	this->max_fds -= 10;

	this->logger = va_arg(* params, Logger);
	this->worker = va_arg(* params, void *);
	port         = va_arg(* params, int);
	backlog      = va_arg(* params, unsigned int);

	loggerLog(this->logger,
			LOGGER_INFO,
			"accept up to %zu connections",
			this->max_fds);

	this->fds   = memCalloc(sizeof(struct pollfd), this->max_fds);
	this->conns = memCalloc(sizeof(struct conns), this->max_fds);

	this->sock = new(Sock, this->logger, port);
	socketNonblock(this->sock);
	flags      = fcntl(this->sock->handle, F_GETFL, 0);
	fcntl(this->sock->handle, F_SETFL, flags | O_NONBLOCK);

	// this->sockSSL = new(Sock, this->logger, port+1);
	// flags         = fcntl(this->sockSSL->handle, F_GETFL, 0);
	// fcntl(this->sockSSL->handle, F_SETFL, flags | O_NONBLOCK);

	// SSL_library_init();
	// SSL_load_error_strings();
	// this->ctx = SSL_CTX_new(SSLv23_server_method());
	// SSL_CTX_use_certificate_file(
	// 		this->ctx,
	// 		"./certs/server.crt",
	// 		SSL_FILETYPE_PEM);

	// SSL_CTX_use_RSAPrivateKey_file(
	// 		this->ctx,
	// 		"./certs/server.key",
	// 		SSL_FILETYPE_PEM);

	socketListen(this->sock, backlog);
	// socketListen(this->sockSSL, backlog);

	(this->fds)[0].fd     = this->sock->handle;
	(this->fds)[0].events = POLLIN;
	// (this->fds)[1].fd     = this->sockSSL->handle;
	// (this->fds)[1].events = POLLIN;
	// this->nfds = 2;
	this->nfds = 1;

	return 0;
}

static
void
serverDtor(void * _this)
{
	Server this = _this;
    int    i;

    for (i=0; i<this->nfds; i++) {
		if (this->sock->handle != (this->fds)[i].fd &&
				this->sockSSL->handle != (this->fds)[i].fd) {
			serverCloseConn(this, i);
		}
    }

	MEM_FREE(this->fds);
	MEM_FREE(this->conns);

	delete(this->sock);
	// delete(this->sockSSL);

	// SSL_CTX_free(this->ctx);
	// ERR_free_strings();
}

INIT_IFACE(Class, serverCtor, serverDtor, NULL);
CREATE_CLASS(Server, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
