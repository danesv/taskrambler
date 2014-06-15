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

#include <trbase.h>
#include <trhash.h>
#include <trdata.h>

#include "application/application.h"
#include "session.h"


#define USER_JSON \
	"{\"email\":\"%s\",\"firstname\":\"%s\",\"surname\":\"%s\"}"

char *
controllerUserRead(Application app, Session sess, TR_Hash args)
{
	char         * buffer;
	size_t         nbuffer;
	TR_HashValue   id     = TR_hashGet(args, CSTRA("id"));
	TR_Uuid        search = TR_uuidParse(id->value);
	User           user   = applicationGetUser(app, search);

	nbuffer = snprintf(NULL, 0, USER_JSON,
			user->email,
			user->firstname,
			user->surname);
	buffer  = TR_malloc(nbuffer);
	nbuffer = sprintf(buffer, USER_JSON,
			user->email,
			user->firstname,
			user->surname);

	return buffer;
}

// vim: set ts=4 sw=4:
