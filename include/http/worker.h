/**
 * \file
 * Worker for processing HTTP request, response cycles.
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

#ifndef __HTTP_WORKER_H__
#define __HTTP_WORKER_H__

#include <sys/types.h>
#include <time.h>

#include "class.h"
#include "http/parser.h"
#include "http/writer.h"
#include "cbuf.h"
#include "session.h"

#include "commons.h"

struct randval {
	time_t timestamp;
	int    value;
};


CLASS(HttpWorker) {
	char           * id;
	struct randval * val;

	Cbuf       pbuf;
	Cbuf       wbuf;

	HttpParser parser;
	HttpWriter writer;
	Session    session;
	Session  * sroot;
};

#endif // __HTTP_WORKER_H__

// vim: set ts=4 sw=4:
