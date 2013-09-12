/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2013  Georg Hopp
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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "class.h"
#include "application/application.h"
#include "application/adapter/http.h"
#include "hash.h"
#include "http/worker.h"
#include "http/header.h"
#include "http/response.h"
#include "auth/credential.h"

#include "utils/memory.h"


#define NO_SESSION_SID		0


static
inline
unsigned long
getSessionId(Hash cookies)
{
	HashValue sidstr = hashGet(cookies, CSTRA("sid"));

	if (NULL != sidstr) {
		return strtoul((char*)(sidstr->value), NULL, 10);
	}

	return NO_SESSION_SID;
}

static
void
loginAdapter(Application application, HttpWorker worker, unsigned long sid)
{
	HashValue  username;
	HashValue  password;
	Credential credential;

	username = hashGet(
			worker->current_request->post,
			CSTRA("username"));
	password = hashGet(
			worker->current_request->post,
			CSTRA("password"));

	if (NULL == username || NULL == password) {
		worker->current_response =
			new(HttpResponse, "HTTP/1.1", 403, "Forbidden");
		return;
	}

	credential = new(Credential,
			CRED_PASSWORD,
			(char *)(username->value), username->nvalue,
			(char *)(password->value), password->nvalue);

	if (applicationLogin(application, credential)) {
		char    buffer[200];
		size_t  nbuf;

		if (NO_SESSION_SID == sid
				|| NULL == applicationSessionGet(application, sid)) {
			sid = applicationSessionStart(
					application,
					(char *)(username->value),
					username->nvalue);
		} else {
			applicationSessionUpdate(
					application,
					sid,
					username->value,
					username->nvalue);
		}

		nbuf = sprintf(buffer, "sid=%lu;Path=/", sid);

		worker->current_response = 
			(HttpMessage)httpResponseSession(
					applicationSessionGet(application, sid));

		hashAdd(
				worker->current_response->header,
				new(HttpHeader, CSTRA("Set-Cookie"), buffer, nbuf));
	} else {
		worker->current_response =
			new(HttpResponse, "HTTP/1.1", 403, "Forbidden");
	}

	delete(credential);
}


void
applicationAdapterHttpUpdate(void * _this, void * subject)
{
	ApplicationAdapterHttp this = _this;
	HttpWorker    worker  = (HttpWorker)subject;
	unsigned long sid     = getSessionId(worker->current_request->cookies);
	Session       session = applicationSessionGet(this->application, sid);

	if (NULL != session) {
		if (time(NULL) < session->livetime) {
			session->livetime = time(NULL) + SESSION_LIVETIME;
		} else {
			applicationSessionStop(this->application, sid);
		}

	}

	if (0 == strcmp("POST", worker->current_request->method)) {
		if (0 == strcmp("/login/", worker->current_request->path)) {
			loginAdapter(this->application, worker, sid);
			return;
		}
	}

	if (0 == strcmp("GET", worker->current_request->method)) {
		if (0 == strcmp("/sessinfo/", worker->current_request->path)) {
			worker->current_response = 
				(HttpMessage)httpResponseSession(
						applicationSessionGet(this->application, sid));
			return;
		}
		
		if (0 == strcmp("/sess/", worker->current_request->path)) {
			if (NO_SESSION_SID == sid
				|| NULL == applicationSessionGet(this->application, sid)) {
				sid = applicationSessionStart(this->application, NULL, 0);
			}

			worker->current_response = 
				(HttpMessage)httpResponseSession(
						applicationSessionGet(this->application, sid));
			return;
		}

		if (0 == strcmp("/randval/", worker->current_request->path)) {
			if (NO_SESSION_SID != sid) {
				worker->current_response = 
					(HttpMessage)httpResponseRandval(
							this->application->val->timestamp,
							this->application->val->value);
			} else {
				worker->current_response = (HttpMessage)httpResponse403();
			}
		}
	}
}

// vim: set ts=4 sw=4:
