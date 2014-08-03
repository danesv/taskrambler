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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

#include "trbase.h"
#include "trdata.h"
#include "trio.h"

#include "application/application.h"
#include "application/adapter/http.h"
#include "http/worker.h"
#include "http/header.h"
#include "http/message.h"
#include "router.h"

#define NO_SESSION_SID		NULL

static
inline
char *
getSessionId(TR_Hash cookies)
{
	TR_HashValue sidstr = TR_hashGet(cookies, CSTRA("sid"));

	if (NULL != sidstr) {
		return (char*)sidstr->value;
	}

	return NO_SESSION_SID;
}

void
applicationAdapterHttpUpdate(void * _this, void * subject)
{
	ApplicationAdapterHttp this    = _this;
	HttpWorker             worker  = (HttpWorker)subject;
	Session                session = NULL;
	time_t                 now     = time(NULL);

	uint32_t   ip;
	char     * sid;
	char       buf[1000];
	size_t     nbuf;

	ip = TR_sdbm(
			TR_socketAddrIp(worker->socket),
			TR_socketAddrIplen(worker->socket));

	applicationSessionCleanup(this->application, now);

	sid = getSessionId(worker->current_request->cookies);
	session = applicationSessionGet(this->application, sid, ip);

	if (NULL == session) {
		session = applicationSessionStart(this->application, ip);
	}

	// send session cookie
	nbuf = sprintf(buf, "sid=%s;Path=/", session->id);
	TR_queuePut(
			worker->additional_headers, 
			TR_new(HttpHeader, CSTRA("Set-Cookie"), buf, nbuf));

	worker->current_response = (HttpMessage)routerRoute(
			this->router,
			worker->current_request,
			session);
}

// vim: set ts=4 sw=4:
