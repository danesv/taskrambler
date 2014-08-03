/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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

#include "trbase.h"
#include "trio.h"
#include "server.h"


void serverCloseConn(Server, unsigned int);


static
int
serverCtor(void * _this, va_list * params)
{
	Server       this = _this;
	in_port_t    port;

	this->max_fds = sysconf(_SC_OPEN_MAX);
	if (this->max_fds <= 10) {	// reserve 10 handles for internal use.
		/**
		 * \todo some logging would be appropriate :)
		 */
		return -1;
	}
	this->max_fds -= 10;

	this->logger = va_arg(* params, TR_Logger);
	this->worker = va_arg(* params, void *);
	port         = va_arg(* params, int);

	TR_loggerLog(this->logger,
			TR_LOGGER_INFO,
			"accept up to %zu connections",
			this->max_fds);

	this->fds   = TR_calloc(sizeof(struct pollfd), this->max_fds);
	this->conns = TR_calloc(sizeof(struct conns), this->max_fds);

	this->sock = TR_new(TR_TcpSocket, this->logger, "::", port, 0);
	TR_socketBind((TR_Socket)this->sock);
	TR_socketNonblock(this->sock);

	this->sockSSL = TR_new(TR_TcpSocket, this->logger, "::", port+1, 0);
	TR_socketBind((TR_Socket)this->sockSSL);
	TR_socketNonblock(this->sockSSL);

	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	this->ctx = SSL_CTX_new(SSLv23_server_method());
	SSL_CTX_set_cipher_list(
			this->ctx,
			"ECDH+AESGCM:DH+AESGCM:ECDH+AES256:DH+AES256:ECDH+AES128:DH+AES:ECDH+3DES:DH+3DES:RSA+AES:RSA+3DES:!ADH:!AECDH:!MD5:!DSS");
	SSL_CTX_use_certificate_file(
			this->ctx,
			CONFIGDIR "/taskrambler.crt",
			SSL_FILETYPE_PEM);

	SSL_CTX_use_RSAPrivateKey_file(
			this->ctx,
			CONFIGDIR "/taskrambler.pem",
			SSL_FILETYPE_PEM);

	(this->fds)[0].fd     = this->sock->handle;
	(this->fds)[0].events = POLLIN;
	(this->fds)[1].fd     = this->sockSSL->handle;
	(this->fds)[1].events = POLLIN;
	this->nfds = 2;

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

	TR_MEM_FREE(this->fds);
	TR_MEM_FREE(this->conns);

	TR_delete(this->sock);
	TR_delete(this->sockSSL);

	SSL_CTX_free(this->ctx);
	ERR_free_strings();
}

TR_INIT_IFACE(TR_Class, serverCtor, serverDtor, NULL);
TR_CREATE_CLASS(Server, NULL, NULL, TR_IF(TR_Class));

// vim: set ts=4 sw=4:
