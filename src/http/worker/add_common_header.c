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

#include <time.h>
#include <sys/types.h>

#include "class.h"

#include "http/message.h"
#include "http/header.h"
#include "http/response.h"

#include "utils/memory.h"
#include "hash.h"

static const char *DAY_NAMES[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


void
httpWorkerAddCommonHeader(HttpMessage request, HttpMessage response)
{
	time_t       t;
	struct tm *  tmp;
	char         buffer[200];
	size_t       nbuf;

	if (httpMessageHasKeepAlive(request)) {
		hashAdd(response->header,
				new(HttpHeader, CSTRA("Connection"), CSTRA("Keep-Alive")));
	}
	else {
		hashAdd(response->header,
				new(HttpHeader, CSTRA("Connection"), CSTRA("Close")));
	}

	hashAdd(response->header,
			new(HttpHeader, CSTRA("Server"), CSTRA("testserver")));

	switch(((HttpResponse)response)->status) {
		case 304:
			break;

		default:
			nbuf = sprintf(buffer, "%d", response->nbody);
			hashAdd(response->header,
					new(HttpHeader, CSTRA("Content-Length"), buffer, nbuf));
	}

	t    = time(NULL);
	tmp  = gmtime(&t);
	nbuf = strftime(buffer, sizeof(buffer), "---, %d --- %Y %T GMT", tmp);
	memcpy(buffer, DAY_NAMES[tmp->tm_wday], 3);
	memcpy(buffer+8, MONTH_NAMES[tmp->tm_mon], 3);

	hashAdd(response->header,
			new(HttpHeader, CSTRA("Date"), buffer, nbuf));
}

// vim: set ts=4 sw=4:
