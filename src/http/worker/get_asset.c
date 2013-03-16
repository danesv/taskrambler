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

#include "http/header.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"

#include "utils/memory.h"
#include "hash.h"

HttpMessage
httpWorkerGetAsset(
		HttpRequest request,
		const char * fname,
		const char * mime,
		size_t       nmime)
{
	char *     match;
	size_t     nmatch;
	HttpHeader header;

	header = hashGet(
			((HttpMessage)request)->header,
			CSTRA("If-None-Match"));

	if (NULL == header) {
		match  = "";
		nmatch = 0;
	}
	else {
		match  = (header->value)[0];
		nmatch = (header->nvalue)[0];
	}

	return (HttpMessage)httpResponseAsset(
			fname, mime, nmime, match, nmatch);
}

// vim: set ts=4 sw=4:
