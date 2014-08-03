/**
 * \file
 * Generic HTTP message. Parent for request and response.
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

#ifndef __HTTP_MESSAGE__
#define __HTTP_MESSAGE__

#include "trbase.h"
#include "trdata.h"
#include "asset.h"

TR_CLASS(HttpMessage) {
	char    * version;

	TR_Hash   header;

	Asset     asset;
	char    * body;

	int       nbody;
	int       dbody;
};
TR_INSTANCE_INIT(HttpMessage);
TR_CLASSVARS_DECL(HttpMessage) {};

char   httpMessageHasKeepAlive(HttpMessage);
size_t httpMessageHeaderSizeGet(HttpMessage);
char * httpMessageHeaderToString(HttpMessage, char *);

int    httpMessageGetVersion(HttpMessage, int *, int *);
int    httpMessageHasValidVersion(HttpMessage);

#endif // __HTTP_MESSAGE__

// vim: set ts=4 sw=4:
