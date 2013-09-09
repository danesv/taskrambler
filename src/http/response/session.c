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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>

#include "class.h"

#include "http/response.h"
#include "http/message.h"
#include "http/header.h"
#include "session.h"

#include "utils/memory.h"
#include "hash.h"

#define RESP_DATA "{\"id\":\"%lu\",\"timeout\":%d,\"timeleft\":%ld,\"username\":\"%s\"}"

HttpResponse
httpResponseSession(Session session)
{
	char         buffer[200];
	HttpResponse response;
	HttpMessage  message;
	size_t       nbuf;

	response = new(HttpResponse, "HTTP/1.1", 200, "OK");
	message  = (HttpMessage)response;

	hashAdd(message->header,
			new(HttpHeader, CSTRA("Content-Type"), CSTRA("application/json")));

	nbuf = sprintf(buffer, RESP_DATA,
			(NULL != session)? session->id : 0,
			(NULL != session)? SESSION_LIVETIME : 0,
			(NULL != session)? session->livetime - time(NULL) : 0,
			(NULL != session)? session->username : "");

	message->nbody = nbuf;
	message->body  = memMalloc(nbuf);
	memcpy(message->body, buffer, nbuf);

	return response;
}

// vim: set ts=4 sw=4:
