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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "class.h"
#include "interface/class.h"

#include "http/response.h"
#include "http/message.h"
#include "http/header.h"


HttpResponse
httpResponse403()
{
	HttpResponse response;
	HttpMessage  message;

	response = new(HttpResponse, "HTTP/1.1", 403, "Forbidden");
	message  = (HttpMessage)response;

	message->type  = HTTP_MESSAGE_BUFFERED;
	message->nbody = 0;
	message->body  = NULL;

	return response;
}

// vim: set ts=4 sw=4:
