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
#include "user.h"

#include "utils/memory.h"


#define NO_SESSION_SID		NULL
#define SESS_HEADER "{\"id\":\"%s\",\"timeout\":%d,\"timeleft\":%ld}"


static
inline
char *
getSessionId(Hash cookies)
{
	HashValue sidstr = hashGet(cookies, CSTRA("sid"));

	if (NULL != sidstr) {
		return (char*)sidstr->value;
	}

	return NO_SESSION_SID;
}

static
void
loginAdapter(Application application, HttpWorker worker, Session session)
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

	if (NULL == username) {
		username = hashGet(
				worker->current_request->post,
				CSTRA("email"));
	}

	if (NULL == username || NULL == password) {
		worker->current_response =
			new(HttpResponse, "HTTP/1.1", 403, "Forbidden");
		return;
	}

	credential = new(Credential,
			CRED_PASSWORD,
			(char *)(username->value), username->nvalue,
			(char *)(password->value), password->nvalue);

	if (! applicationLogin(application, credential, session)) {
		worker->current_response =
			new(HttpResponse, "HTTP/1.1", 403, "Forbidden");
	} else {
		worker->current_response =
			(HttpMessage)httpResponseUser(session->user);
	}

	delete(credential);
}

static
void
signupAdapter(Application application, HttpWorker worker, Session session)
{
	HashValue email;
	HashValue password;
	HashValue pwrepeat;
	HashValue firstname;
	HashValue surname;

	Credential credential;
	User       user;

	email = hashGet(
			worker->current_request->post,
			CSTRA("email"));
	password = hashGet(
			worker->current_request->post,
			CSTRA("password"));
	pwrepeat = hashGet(
			worker->current_request->post,
			CSTRA("pwrepeat"));
	firstname = hashGet(
			worker->current_request->post,
			CSTRA("firstname"));
	surname = hashGet(
			worker->current_request->post,
			CSTRA("surname"));

	if (
			NULL == email ||
			NULL == password ||
			NULL == pwrepeat ||
			NULL == firstname ||
			NULL == surname) {
		// maybe this is not a 500...have to check repsonse codes.
		worker->current_response = (HttpMessage)httpResponse500();
		return;
	}

	if (password->nvalue != pwrepeat->nvalue ||
			0 != memcmp(password->value, pwrepeat->value, password->nvalue)) {
		// maybe this is not a 500...have to check repsonse codes.
		worker->current_response = (HttpMessage)httpResponse500();
		return;
	}

	credential = new(Credential,
			CRED_PASSWORD,
			(char *)(email->value), email->nvalue,
			(char *)(password->value), password->nvalue);

	user = new(User,
			(char *)(email->value), email->nvalue,
			(char *)(firstname->value), firstname->nvalue,
			(char *)(surname->value), surname->nvalue);

	if (! applicationSignup(application, credential, user, session)) {
		worker->current_response = (HttpMessage)httpResponse500();
	} else {
		loginAdapter(application, worker, session);
	}

	delete(credential);
	delete(user);
}


void
applicationAdapterHttpUpdate(void * _this, void * subject)
{
	ApplicationAdapterHttp this = _this;
	HttpWorker worker  = (HttpWorker)subject;
	char *     sid;
	Session    session;
	char       buf[200];
	size_t     nbuf;

	sid     = getSessionId(worker->current_request->cookies);

	session = applicationSessionGet(this->application, sid);
	if (NULL == session) {
		session = applicationSessionStart(this->application);
	}

	nbuf = sprintf(buf, SESS_HEADER,
			session->id, 
			SESSION_LIVETIME,
			session->livetime - time(NULL));
	queuePut(
			worker->additional_headers, 
			new(HttpHeader, CSTRA("X-TaskramblerSession"), buf, nbuf));

	nbuf = sprintf(buf, "sid=%s;Path=/", session->id);
	queuePut(
			worker->additional_headers, 
			new(HttpHeader, CSTRA("Set-Cookie"), buf, nbuf));

	if (0 == strcmp("POST", worker->current_request->method)) {
		if (0 == strcmp("/login/", worker->current_request->path)) {
			loginAdapter(this->application, worker, session);
			return;
		}

		if (0 == strcmp("/signup/", worker->current_request->path)) {
			signupAdapter(this->application, worker, session);
			return;
		}
	}

	if (0 == strcmp("GET", worker->current_request->method)) {
		if (0 == strcmp("/version/", worker->current_request->path)) {
			worker->current_response = 
				(HttpMessage)httpResponseVersion(this->application->version);
			return;
		}

		if (0 == strcmp("/user/get/", worker->current_request->path)) {
			worker->current_response = 
				(HttpMessage)httpResponseUser(session->user);
			return;
		}

//		if (0 == strcmp("/sess/", worker->current_request->path)) {
//			if (NO_SESSION_SID == sid
//					|| NULL == applicationSessionGet(this->application, sid)) {
//				sid = applicationSessionStart(this->application, NULL, 0);
//			}
//
//			worker->current_response = 
//				(HttpMessage)httpResponseSession(
//						applicationSessionGet(this->application, sid));
//			return;
//		}

		if (0 == strcmp("/randval/", worker->current_request->path)) {
			if (NULL != session->user) {
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
