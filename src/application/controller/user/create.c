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

#include "application/application.h"
#include "session.h"
#include "hash.h"
#include "auth/credential.h"
#include "user.h"

#include "utils/memory.h"

char * controllerCurrentuserRead(Application, Session, Hash);

char *
controllerUserCreate(
		Application application,
		Session     session,
		Hash        args)
{
	HashValue email;
	HashValue password;
	HashValue pwrepeat;
	HashValue firstname;
	HashValue surname;

	Credential credential;
	User       user;

	char * response_data;

	email     = hashGet(args, CSTRA("email"));
	password  = hashGet(args, CSTRA("password"));
	pwrepeat  = hashGet(args, CSTRA("pwrepeat"));
	firstname = hashGet(args, CSTRA("firstname"));
	surname   = hashGet(args, CSTRA("surname"));

	if (
			NULL == email ||
			NULL == password ||
			NULL == pwrepeat ||
			NULL == firstname ||
			NULL == surname)
	{
		return NULL;
	}

	if (
			password->nvalue != pwrepeat->nvalue ||
			0 != memcmp(password->value, pwrepeat->value, password->nvalue))
	{
		return NULL;
	}

	credential = new(Credential,
			CRED_PASSWORD,
			(char *)(email->value), email->nvalue,
			(char *)(password->value), password->nvalue);

	user = new(User,
			(char *)(email->value), email->nvalue,
			(char *)(firstname->value), firstname->nvalue,
			(char *)(surname->value), surname->nvalue);

	if (! applicationSignup(application, credential, user, session)) {
		response_data = NULL;
	} else {
		applicationLogin(application, credential, session);
		response_data = controllerCurrentuserRead(application, session, NULL);
	}

	delete(credential);
	delete(user);

	return response_data;
}

// vim: set ts=4 sw=4:
