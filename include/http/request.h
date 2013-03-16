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

#include "class.h"
#include "http/message.h"
#include "hash.h"

#define N_HTTP_METHOD	8

char * http_method[N_HTTP_METHOD];

CLASS(HttpRequest) {
	EXTENDS(HttpMessage);

	char *     method;
	char *     uri;
	char *     path;

	Hash       get;
	Hash       post;
	Hash       cookies;
};

int httpRequestHasValidMethod(HttpRequest);

#endif // __HTTP_REQUEST_H__

// vim: set ts=4 sw=4:
