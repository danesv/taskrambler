/**
 * \file
 * Worker for processing HTTP request, response cycles.
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

#ifndef __HTTP_WORKER_H__
#define __HTTP_WORKER_H__

#include <sys/types.h>
#include <time.h>

#include "trbase.h"
#include "trdata.h"
#include "http/parser.h"
#include "http/writer.h"
#include "session.h"

#include "http/request.h"
#include "http/response.h"


TR_CLASS(HttpWorker) {
	char        * id;

	TR_Cbuf       pbuf;
	TR_Hash       asset_pool;

	void        * application_adapter;

	HttpRequest   current_request;
	HttpMessage   current_response;

	TR_Queue      additional_headers;

	HttpParser    parser;
	HttpWriter    writer;
};

#endif // __HTTP_WORKER_H__

// vim: set ts=4 sw=4:
