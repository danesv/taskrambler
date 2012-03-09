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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "class.h"
#include "interface/class.h"

#include "http/worker.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"
#include "http/message/queue.h"
#include "http/parser.h"
#include "session.h"
#include "stream.h"

#include "utils/memory.h"

HttpMessage httpWorkerGetAsset(HttpRequest, const char *, const char *, size_t);
void        httpWorkerAddCommonHeader(HttpMessage, HttpMessage);

ssize_t
httpWorkerProcess(HttpWorker this, Stream st)
{
	ssize_t size;

	if (0 < (size = httpParserParse(this->parser, st))) {
		int              i;
		HttpMessageQueue reqq  = this->parser->queue;
		HttpMessageQueue respq = this->writer->queue;

		for (i=0; i<reqq->nmsgs; i++) {
			HttpMessage  rmessage = reqq->msgs[i];
			HttpRequest  request  = (HttpRequest)(reqq->msgs[i]);
			HttpMessage  response = NULL;
			HttpHeader   cookie   = httpHeaderGet(
					&(rmessage->header),
					CSTRA("cookie"));

			if (NULL == this->session && NULL != cookie) {
				int i;

				for (i=0; i<cookie->cvalue; i++) {
					char * sidstr = strstr(cookie->value[i], "sid");

					if (NULL != sidstr) {
						unsigned long sid;

						sidstr = strchr(sidstr, '=')+1;
						sid    = strtoul(sidstr, NULL, 10);

						this->session = sessionGet(this->sroot, sid);
						break;
					}
				}
			}

			if (NULL != this->session) {
				if (time(NULL) < this->session->livetime) {
					this->session->livetime = time(NULL) + SESSION_LIVETIME;
				} else {
					sessionDelete(this->sroot, this->session->id);
					delete(this->session);
				}
			}

			if (0 == strcmp("POST", request->method)) {
				if (0 == strcmp("/me/", request->uri)) {
					char * delim = memchr(rmessage->body, '=', rmessage->nbody);
					char * val;
					size_t nkey, nval;
					char   buffer[200];
					size_t nbuf;

					nkey = delim - rmessage->body - 1;
					*delim = 0;
					val  = delim + 1;
					nval = rmessage->nbody - (val - rmessage->body);

					this->session = sessionAdd(
							this->sroot,
							new(Session, val, nval));
					nbuf = sprintf(buffer, "sid=%lu;Path=/", this->session->id);

					response = (HttpMessage)httpResponseMe(this->session->username);

					httpHeaderAdd(
							&(response->header),
							new(HttpHeader, CSTRA("Set-Cookie"), buffer, nbuf));
				}
			}

			if (0 == strcmp("GET", request->method)) {

				if (0 == strcmp("/login/", request->uri)) {
					response = (HttpMessage)httpResponseLoginForm();
				}

				if (0 == strcmp("/me/", request->uri)) {
					char * uname = (NULL != this->session)? this->session->username : "";
					response = (HttpMessage)httpResponseMe(uname);
				}

				if (0 == strcmp("/randval/", request->uri)) {
					if (NULL != this->session) {
						response = (HttpMessage)httpResponseRandval(
								this->val->timestamp,
								this->val->value);
					} else {
						response = (HttpMessage)httpResponse403();
					}
				}

				if (0 == strcmp("/image/", request->uri)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/waldschrat.jpg",
							CSTRA("image/jpeg"));
				}

				if (0 == strcmp("/jquery/", request->uri)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/jquery-1.7.1.min.js",
							CSTRA("text/javascript"));
				}
			}

			if (NULL == response) {
				response = (HttpMessage)httpResponse404();
			}

			httpWorkerAddCommonHeader((HttpMessage)request, response);

			respq->msgs[(respq->nmsgs)++] = response;
			response = NULL;
			delete((reqq->msgs)[i]);
		}

		reqq->nmsgs = 0;
	}

	return size;
}

// vim: set ts=4 sw=4:
