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

#include <sys/types.h>
#include <time.h>

#include "trbase.h"
#include "trdata.h"
#include "http/header.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"
#include "http/worker.h"

HttpMessage
httpWorkerGetAsset(HttpWorker this, const char * fname)
{
	char *      match;
	size_t      nmatch;
	HttpHeader  header;
	HttpMessage message;

	size_t nfname = strlen(fname);

	header = TR_hashGet(
			((HttpMessage)this->current_request)->header,
			CSTRA("If-None-Match"));

	if (NULL == header) {
		match  = "";
		nmatch = 0;
	}
	else {
		match  = (header->value)[0];
		nmatch = (header->nvalue)[0];
	}

	/*
	 * assets expire after 12 hours...
	 */
	message = (HttpMessage)httpResponseAsset(
			fname,
			nfname,
			time(NULL) + 43200);

	if (NULL == message) {
		return (HttpMessage)httpResponse404();
	}

	if (message->asset->netag == nmatch
			&& 0 == memcmp(message->asset->etag, match, nmatch)) {
		HttpMessage new_message;

		new_message = (HttpMessage)httpResponse304(
				message->asset->mime_type, message->asset->nmime_type,
				message->asset->etag, message->asset->netag,
				message->asset->mtime, message->asset->nmtime);

		TR_delete(message);

		return new_message;
	}   

	return message;
}

// vim: set ts=4 sw=4:
