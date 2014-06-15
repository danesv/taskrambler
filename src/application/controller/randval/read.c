/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2013  Georg Hopp
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

#define _GNU_SOURCE

#include <sys/types.h>
#include <stdio.h>

#include "trbase.h"
#include "trdata.h"

#include "application/application.h"
#include "session.h"

#define RANDVAL_JSON	"{\"ctime\":%ld,\"vnext\":%ld,\"value\":\"%02d\"}"


char *
controllerRandvalRead(Application app, Session sess, TR_Hash args)
{
	char   * buffer;
	size_t   nbuffer;
	time_t   remaining;

	remaining = 10 - (time(NULL) - app->val->timestamp);

	nbuffer = snprintf(
			NULL,
			0,
			RANDVAL_JSON,
			app->val->timestamp,
			remaining,
			app->val->value);
	buffer  = TR_malloc(nbuffer);
	sprintf(
			buffer,
			RANDVAL_JSON,
			app->val->timestamp,
			remaining,
			app->val->value);

	return buffer;
}

// vim: set ts=4 sw=4:
