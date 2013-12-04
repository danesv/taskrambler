/**
 * \file
 * A response class that delivers an asset (file on disk).
 *
 * In future this will use a asset class, get from an asset class
 * hash. The asset hash will be a shared resource between all
 * workers.
 *
 * The asset class holds an open file descriptor wich is memory
 * mapped and is able to give the correct pointer to neede data.
 *
 * This change will envolve changes in other parts of the response
 * write system, as we no longer need to destinguish between piped
 * and bufferd resources...we will allways work with a memory address
 * only one time its allocated and one time a memory mapped file.
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

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

#include "trbase.h"
#include "stream.h"
#include "http/response.h"
#include "http/message.h"
#include "http/header.h"
#include "utils/http.h"
#include "hash.h"

#include "asset.h"


HttpResponse
httpResponseAsset(const char * fname, size_t nfname, time_t exptime)
{
	HttpResponse response;
	HttpMessage  message;
	Asset        asset = assetPoolGet(fname, nfname);
	char         expires[200];
	size_t       nexpires;

	if (NULL == asset) {
		return NULL;
	}

	response = TR_new(HttpResponse, "HTTP/1.1", 200, "OK");
	message  = (HttpMessage)response;

	message->asset = asset;
	message->body  = asset->data;
	message->nbody = asset->size;

	nexpires = rfc1123Gmt(expires, sizeof(expires), &exptime);

	hashAdd(message->header,
			TR_new(HttpHeader, CSTRA("Content-Type"),
				asset->mime_type, asset->nmime_type));
	hashAdd(message->header,
			TR_new(HttpHeader, CSTRA("ETag"), asset->etag, asset->netag));
	hashAdd(message->header,
			TR_new(HttpHeader, CSTRA("Expires"), expires, nexpires));
	hashAdd(message->header,
			TR_new(HttpHeader, CSTRA("Last-Modified"),
				asset->mtime, asset->nmtime));

	return response;
}

// vim: set ts=4 sw=4:
