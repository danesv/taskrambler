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
#include "queue.h"
#include "session.h"
#include "stream.h"
#include "hash.h"

#include "http/worker.h"
#include "http/header.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"
#include "http/parser.h"

#include "utils/memory.h"
#include "commons.h"


HttpMessage httpWorkerGetAsset(HttpRequest, const char *, const char *, size_t);
void        httpWorkerAddCommonHeader(HttpMessage, HttpMessage);
char *		httpWorkerGetMimeType(HttpWorker, const char * extension);


ssize_t
httpWorkerProcess(HttpWorker this, Stream st)
{
	ssize_t size;

	if (0 < (size = httpParserParse(this->parser, st))) {

		while (! queueEmpty(this->parser->queue)) {
			HttpRequest request  = queueGet(this->parser->queue);
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
								this->session->username = memMalloc(username->nvalue + 1);
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
				if (0 == strcmp("/sessinfo/", request->path)) {
					response = (HttpMessage)httpResponseSession(this->session);
				}

				else if (0 == strcmp("/sess/", request->path)) {
					if (NULL == this->session) {
						this->session = sessionAdd(
								this->sroot,
								new(Session, NULL, 0));
					}
					response = (HttpMessage)httpResponseSession(this->session);
				}

				else if (0 == strcmp("/randval/", request->path)) {
					if (NULL != this->session) {
						response = (HttpMessage)httpResponseRandval(
								this->val->timestamp,
								this->val->value);
					} else {
						response = (HttpMessage)httpResponse403();
					}
				}

				else if (0 == strcmp("/assets/js/serverval", request->path)) {
					response = httpWorkerGetAsset(
							request,
							"./assets/js/serverval.js",
							CSTRA("text/javascript"));
				}

				else {
					char html_asset[2048] = "./assets/html";
					char base_asset[2048] = "./assets";
					char main_asset[]     = "/main.html";

					char * mime_type      = NULL;
					char   default_mime[] = "application/octet-stream";
					char * asset_path     = base_asset;
					char * asset;
					char * extension;

					if (0 == strcmp("/", request->path)) {
						asset = main_asset;
					} else {
						asset = request->path;
					}

					extension = strrchr(asset, '.');

					if (NULL != extension) {
						extension++;
						mime_type = httpWorkerGetMimeType(this, extension);
					}

					if (NULL != mime_type &&
							0 == memcmp(mime_type, CSTRA("text/html"))) {
						asset_path = html_asset;
					}

					if (NULL == mime_type) {
						mime_type = default_mime;
					}

					strcat(asset_path, asset);
					response = httpWorkerGetAsset(
							request,
							asset_path,
							mime_type,
							strlen(mime_type));
				}

			}

			if (NULL == response) {
				response = (HttpMessage)httpResponse404();
			}

			httpWorkerAddCommonHeader((HttpMessage)request, response);

			delete(request);

			queuePut(this->writer->queue, response);
			response = NULL;
		}
	}

	return size;
}

// vim: set ts=4 sw=4:
