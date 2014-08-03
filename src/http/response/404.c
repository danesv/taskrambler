/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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

#include "trbase.h"
#include "trdata.h"
#include "http/response.h"
#include "http/message.h"
#include "http/header.h"

#define RESP_DATA "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n" \
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n" \
	" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n" \
	"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n" \
	"<head><title>404 - Not Found</title></head>" \
	"<body><h1>404 - Not Found</h1></body>" \
	"</html>"


HttpResponse
httpResponse404()
{
	HttpResponse response;
	HttpMessage  message;

	response = TR_new(HttpResponse, "HTTP/1.1", 404, "Not Found");
	message  = (HttpMessage)response;

	TR_hashAdd(message->header,
			TR_new(HttpHeader, CSTRA("Content-Type"), CSTRA("text/html")));

	message->nbody = sizeof(RESP_DATA) - 1;
	message->body  = TR_malloc(sizeof(RESP_DATA));
	memcpy(message->body, RESP_DATA, sizeof(RESP_DATA));

	return response;
}

// vim: set ts=4 sw=4:
