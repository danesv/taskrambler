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

#include "http/message.h"
#include "http/header.h"
#include "http/worker.h"

#include "hash.h"

#include "utils/memory.h"
#include "utils/http.h"

#include "config.h"


void
httpWorkerAddCommonHeader(HttpWorker this)
{
	char   buffer[200];
	size_t nbuf;

	if (httpMessageHasKeepAlive((HttpMessage)this->current_request)) {
		hashAdd(this->current_response->header,
				new(HttpHeader, CSTRA("Connection"), CSTRA("Keep-Alive")));
	}
	else {
		hashAdd(this->current_response->header,
				new(HttpHeader, CSTRA("Connection"), CSTRA("Close")));
	}

	hashAdd(this->current_response->header,
			new(HttpHeader, CSTRA("Server"), CSTRA(PACKAGE_STRING)));

	switch(((HttpResponse)this->current_response)->status) {
		case 304:
			break;

		default:
			nbuf = sprintf(buffer, "%d", this->current_response->nbody);
			hashAdd(this->current_response->header,
					new(HttpHeader, CSTRA("Content-Length"), buffer, nbuf));
	}

	nbuf = rfc1123GmtNow(buffer, sizeof(buffer));
	hashAdd(this->current_response->header,
			new(HttpHeader, CSTRA("Date"), buffer, nbuf));
}

// vim: set ts=4 sw=4:
