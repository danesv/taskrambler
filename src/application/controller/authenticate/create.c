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

#include "trbase.h"
#include "trdata.h"

#include "application/application.h"
#include "session.h"
#include "auth/credential.h"
#include "user.h"


char * controllerCurrentuserRead(Application, Session, TR_Hash);

char *
controllerAuthenticateCreate(
		Application application,
		Session     session,
		TR_Hash     args)
{
	TR_HashValue   username;
	TR_HashValue   password;
	Credential     credential;

	char         * response_data;

	username = TR_hashGet(args, CSTRA("username"));
	password = TR_hashGet(args, CSTRA("password"));

	if (NULL == username) {
		username = TR_hashGet(args, CSTRA("email"));
	}

	if (NULL == username || NULL == password) {
		return NULL;
	}

	credential = TR_new(Credential,
			CRED_PASSWORD,
			(char *)(username->value), username->nvalue,
			(char *)(password->value), password->nvalue);

	if (! applicationLogin(application, credential, session)) {
		response_data = NULL;
	} else {
		response_data = controllerCurrentuserRead(application, session, NULL);
	}

	TR_delete(credential);

	return response_data;
}

// vim: set ts=4 sw=4:
