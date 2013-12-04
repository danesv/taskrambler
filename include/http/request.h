/**
 * \file
 * Represents one HTTP request.
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

#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include "trbase.h"
#include "http/message.h"
#include "hash.h"

#define N_HTTP_METHOD	8

extern char * http_method[N_HTTP_METHOD];

typedef enum e_HttpMethod {
	HTTP_OPTIONS = 0,
	HTTP_GET,
	HTTP_HEAD,
	HTTP_POST,
	HTTP_PUT,
	HTTP_DELETE,
	HTTP_TRACE,
	HTTP_CONNECT
} HttpMethod;

TR_CLASS(HttpRequest) {
	TR_EXTENDS(HttpMessage);

	char *     method;
	char *     uri;
	char *     path;

	HttpMethod method_id;

	Hash       get;
	Hash       post;
	Hash       cookies;
};

HttpMethod httpRequestGetMethodId(HttpRequest);

#endif // __HTTP_REQUEST_H__

// vim: set ts=4 sw=4:
