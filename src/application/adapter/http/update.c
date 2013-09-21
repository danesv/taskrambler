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
Session
getSession(Queue sess_queue, const char * sid)
{
	Session sess = NULL;
	time_t  now  = time(NULL);

	/**
	 * session start or update
	 *
	 * @TODO
	 * I need to walk through the whole hash at this point to find
	 * expired sessions and remove them....this is not good and
	 * probably I need another(faster) way to identify expired
	 * sessions....
	 *
	 * @TODO
	 * Build a way to cleanup the hash by a filter...currently 
	 * there is nothing I could use for this.
	 * Well this is practically impossible in reasonable time
	 * because every time I remove one element the tree has to 
	 * be rebalanced....
	 * 
	 * I have to store all nodes in a different structure that
	 * gives me the possibility to find fast expired objects.
	 * These can then be removed from both structures....
	 *
	 * Anyway this is the pure horror...because I have to compute
	 * the condition for every stored session....and I really have to
	 * do this...else the tree would grow and grow all the time...
	 * 
	 * I think the best I can do at this point is, at least for the moment,
	 * to store the sessions in a list and not in a stack.
	 * Each request will than have to walk through that list, 
	 * remove expired sessions and pick out its own....
	 * this is O(n), but currently I gave no better idea at all.
	 */
	while (NULL != sess_queue->next) {
		Session session = (Session)sess_queue->next->msg;

		if (now >= session->livetime) {
			Queue to_delete  = sess_queue->next;
			sess_queue->next = sess_queue->next->next;
			delete(session);
			delete(to_delete);
			continue;
		}

		if (NULL != sid && 0 == memcmp(session->id, sid, 36)) {
			session->livetime = time(NULL) + SESSION_LIVETIME;
			sess              = session;
		}

		sess_queue = sess_queue->next;
	}

	if (NULL == sess) {
		sess = new(Session);
		queuePut(sess_queue, sess);
	}

	return sess;
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
	session = getSession(this->application->active_sessions, sid);

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
	}

	if (0 == strcmp("GET", worker->current_request->method)) {
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
