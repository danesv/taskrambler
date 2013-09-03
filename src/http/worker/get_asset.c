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

#include "http/header.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"
#include "http/worker.h"

#include "utils/memory.h"
#include "hash.h"

HttpMessage
httpWorkerGetAsset(
		HttpWorker   this,
		HttpRequest  request,
		const char * fname)
{
	char *      match;
	size_t      nmatch;
	HttpHeader  header;
	HttpMessage message;
	Asset       asset;

	size_t nfname = strlen(fname);

	header = hashGet(
			((HttpMessage)request)->header,
			CSTRA("If-None-Match"));

	if (NULL == header) {
		match  = "";
		nmatch = 0;
	}
	else {
		match  = (header->value)[0];
		nmatch = (header->nvalue)[0];
	}

	asset = assetPoolGet(fname, nfname);

	if (NULL == asset) {
		return (HttpMessage)httpResponse404();
	}

	if (asset->netag == nmatch
			&& 0 == memcmp(asset->etag, match, asset->netag)) {
		assetPoolRelease(asset);

		return (HttpMessage)httpResponse304(
				asset->mime_type, asset->nmime_type,
				asset->etag, asset->netag,
				asset->mtime, asset->nmtime);
	}   

	message = (HttpMessage)httpResponseAsset(asset);

	if (NULL == message) {
		// here we should be somewhat more care about what causes
		// the message to be NULL... here this could be also a 
		// 404 not found....
		assetPoolRelease(asset);
		message = (HttpMessage)httpResponse500();
	}

	return message;
}

// vim: set ts=4 sw=4:
