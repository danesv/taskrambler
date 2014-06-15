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

#include "trbase.h"
#include "trdata.h"
#include "http/response.h"
#include "http/message.h"
#include "http/header.h"
#include "session.h"

HttpResponse
httpResponseJson(const char * body, size_t nbody)
{
	HttpResponse response;
	HttpMessage  message;

	response = TR_new(HttpResponse, "HTTP/1.1", 200, "OK");
	message  = (HttpMessage)response;

	TR_hashAdd(message->header,
			TR_new(
				HttpHeader,
				CSTRA("Content-Type"),
				CSTRA("application/json")));

	message->nbody = nbody;
	message->body  = TR_malloc(nbody);
	memcpy(message->body, body, nbody);

	return response;
}

// vim: set ts=4 sw=4:
