/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
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

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

#include "class.h"
#include "interface/class.h"
#include "stream.h"

#include "http/response.h"
#include "http/message.h"
#include "http/header.h"

#include "utils/memory.h"


HttpResponse
httpResponseAsset(
		const char * fname,
		const char * mime,
		size_t       nmime,
		const char * match,
		size_t       nmatch)
{
	struct tm *  tmp;
	char         etag[200];
	size_t       netag;
	char         mtime[200];
	size_t       nmtime;
	struct stat  st;
	HttpResponse response;
	HttpMessage  message;
	int          handle;

	handle = open(fname, O_RDONLY);
	fstat(handle, &st);

	tmp    = localtime(&(st.st_mtime));
	netag  = strftime(etag, sizeof(etag), "%s", tmp);
	nmtime = strftime(mtime, sizeof(mtime), "%a, %d %b %Y %T %Z", tmp);

	if (netag == nmatch && 0 == memcmp(etag, match, netag)) {
		return httpResponse304(mime, nmime, etag, netag, mtime, nmtime);
	}

	response = new(HttpResponse, "HTTP/1.1", 200, "OK");
	message  = (HttpMessage)response;

	message->type   = HTTP_MESSAGE_PIPED;
	message->handle = new(Stream, STREAM_FD, handle);
	message->nbody  = st.st_size;

	httpHeaderAdd(&(message->header),
			new(HttpHeader, CSTRA("Content-Type"), mime, nmime));
	httpHeaderAdd(&(message->header),
			new(HttpHeader, CSTRA("ETag"), etag, netag));
	httpHeaderAdd(&(message->header),
			new(HttpHeader, CSTRA("Last-Modified"), mtime, nmtime));

	return response;
}

// vim: set ts=4 sw=4:
