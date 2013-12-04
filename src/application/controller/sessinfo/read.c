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

#include <stdio.h>
#include <sys/types.h>

#include "trbase.h"
#include "application/application.h"
#include "session.h"
#include "hash.h"

#define SESSION_JSON	"{\"id\":\"%s\",\"timeout\":%d,\"timeleft\":%ld}"


char *
controllerSessinfoRead(
		Application application,
		Session     session,
		Hash        args)
{
	char   * buffer;
	size_t   nbuffer;

	nbuffer = snprintf(NULL, 0, SESSION_JSON,
			(NULL != session)? session->id : "",
			(NULL != session)? SESSION_LIVETIME : 0,
			(NULL != session)? session->livetime - time(NULL) : 0);
	buffer  = TR_malloc(nbuffer);
	sprintf(buffer, SESSION_JSON,
			(NULL != session)? session->id : "",
			(NULL != session)? SESSION_LIVETIME : 0,
			(NULL != session)? session->livetime - time(NULL) : 0);

	return buffer;
}

// vim: set ts=4 sw=4:
