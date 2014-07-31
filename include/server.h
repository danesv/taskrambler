/**
 * \file
 * Server class. Handles acceptance and closing of connection and uses
 * a class that has to implement the stream reader and writer interface
 * to handle incoming reads and outgoing writes.
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

#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>	// for printf() and fprintf()
#include <poll.h> 	// for poll system call and related

#include <openssl/ssl.h>

#include "trbase.h"
#include "trio.h"

struct conns {
	TR_Socket   sock;
	TR_Stream   stream;
	void      * worker;
};

TR_CLASS(Server) {
	TR_Logger       logger;
	TR_Socket       sock;
	TR_Socket       sockSSL;
	SSL_CTX       * ctx;
	void          * worker;

	nfds_t          nfds;
	struct pollfd * fds;
	long            max_fds;

	struct conns  * conns;
};
TR_INSTANCE_INIT(Server);

void serverRun(Server this);

#endif // __SERVER_H__

// vim: set ts=4 sw=4:
