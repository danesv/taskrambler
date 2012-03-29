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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include "class.h"
#include "auth.h"

#include "http/worker.h"
#include "http/header.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"
#include "http/message/queue.h"
#include "http/parser.h"
#include "session.h"
#include "stream.h"
#include "hash.h"

#include "utils/memory.h"
#include "hash.h"
#include "commons.h"


HttpMessage httpWorkerGetAsset(HttpRequest, const char *, const char *, size_t);
void        httpWorkerAddCommonHeader(HttpMessage, HttpMessage);


ssize_t
httpWorkerProcess(HttpWorker this, Stream st)
{
	ssize_t size;

	if (0 < (size = httpParserParse(this->parser, st))) {

		while (! httpMessageQueueEmpty(this->parser->queue)) {
			HttpRequest request  = (HttpRequest)httpMessageQueueGet(
					this->parser->queue);
			HttpMessage response = NULL;

			/**
			 * \todo store the cookie count in the request to make a simple
			 * check possible to prevent this lookup if no cookies exists
			 * at all
			 */
			if (NULL == this->session) {
				HashValue sidstr = hashGet(request->cookies, CSTRA("sid"));

					if (NULL != sidstr) {
						unsigned long sid;

					sid = strtoul((char*)(sidstr->value), NULL, 10);
						this->session = sessionGet(this->sroot, sid);
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
				if (0 == strcmp("/login/", request->path)) {
					char   buffer[200];
					size_t nbuf;

					HashValue username = hashGet(request->post, CSTRA("username"));
					HashValue password = hashGet(request->post, CSTRA("password"));

					/**
					 * \todo This is an application authorization not an HTTP
					 * authorization...anyway think about sending HTTP 401
					 * messages if authorization is required and think about
					 * sending the credentials via header as described in the
					 * HTTP protocol. Most likely this will lead to hacky thing
					 * with javascript as i am not sure how far this is implemented
					 * within browsers.
					 * Anyway, for now we simply ignore a failed login within the
					 * response except that no session is initialized. We send
					 * an empty 200 OK
					 */
					if (NULL == password || NULL == username) {
						response = new(HttpResponse, "HTTP/1.1", 403, "Forbidden");
					}

					if (NULL == response) {
						Credential cred = new(Credential,
								CRED_PASSWORD,
								(char*)(username->value), username->nvalue,
								(char*)(password->value), password->nvalue);

						if (!authenticate(this->auth, cred)) {
							response = new(HttpResponse, "HTTP/1.1", 403, "Forbidden");
						} else {
							if (NULL == this->session) {
					this->session = sessionAdd(
							this->sroot,
										new(Session,
											username->value,
											username->nvalue));
							} else {
								this->session->username = malloc(username->nvalue + 1);
								this->session->username[username->nvalue] = 0;
								memcpy(this->session->username,
										username->value,
										username->nvalue);
							}

							nbuf = sprintf(buffer,
									"sid=%lu;Path=/",
									this->session->id);

							response = (HttpMessage)httpResponseSession(
									this->session);

							hashAdd(response->header,
									new(HttpHeader,
										CSTRA("Set-Cookie"),
										buffer, nbuf));
						}
						delete(cred);
					}
				}
			}

			if (0 == strcmp("GET", request->method)) {

				if (0 == strcmp("/", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/html/main.html",
							CSTRA("text/html"));
				}

				if (0 == strcmp("/sessinfo/", request->path)) {
					response = (HttpMessage)httpResponseSession(this->session);
				}

				if (0 == strcmp("/sess/", request->path)) {
					if (NULL == this->session) {
						this->session = sessionAdd(
								this->sroot,
								new(Session, NULL, 0));
					}
					response = (HttpMessage)httpResponseSession(this->session);
				}

				if (0 == strcmp("/randval/", request->path)) {
					if (NULL != this->session) {
						response = (HttpMessage)httpResponseRandval(
								this->val->timestamp,
								this->val->value);
					} else {
						response = (HttpMessage)httpResponse403();
					}
				}

				if (0 == strcmp("/image/me", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/image/waldschrat.jpg",
							CSTRA("image/jpeg"));
				}

				if (0 == strcmp("/assets/js/jquery", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/js/jquery-1.7.1.min.js",
							CSTRA("text/javascript"));
				}

				if (0 == strcmp("/assets/js/serverval", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/js/serverval.js",
							CSTRA("text/javascript"));
				}

				if (0 == strcmp("/assets/js/session", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/js/session.js",
							CSTRA("text/javascript"));
				}

				if (0 == strcmp("/assets/js/init", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/js/init.js",
							CSTRA("text/javascript"));
				}

				if (0 == strcmp("/assets/style/common", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/style/common.css",
							CSTRA("text/css"));
				}
			}

			if (NULL == response) {
				response = (HttpMessage)httpResponse404();
			}

			httpWorkerAddCommonHeader((HttpMessage)request, response);

			delete(request);

			httpMessageQueuePut(this->writer->queue, response);
			response = NULL;
		}
	}

	return size;
}

// vim: set ts=4 sw=4:
