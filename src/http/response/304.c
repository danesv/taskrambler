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

#include <sys/types.h>

#include "class.h"

#include "http/response.h"
#include "http/message.h"
#include "http/header.h"

#include "utils/memory.h"
#include "hash.h"

HttpResponse
httpResponse304(
		const char * mime, size_t nmime,
		const char * etag, size_t netag,
		const char * mtime, size_t nmtime)
{
	HttpResponse response;
	HttpMessage  message;

	response = new(HttpResponse, "HTTP/1.1", 304, "Not Modified");
	message  = (HttpMessage)response;

	message->type  = HTTP_MESSAGE_BUFFERED;
	message->nbody = 0;
	message->body  = NULL;

	hashAdd(message->header,
			new(HttpHeader, CSTRA("Content-Type"), mime, nmime));
	hashAdd(message->header,
			new(HttpHeader, CSTRA("ETag"), etag, netag));
	hashAdd(message->header,
			new(HttpHeader, CSTRA("Last-Modified"), mtime, nmtime));

	return response;
}

// vim: set ts=4 sw=4:
