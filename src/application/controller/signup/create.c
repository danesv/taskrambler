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
int   _controllerProcessUserCreateArgs(Hash, User *, Credential *);


char *
controllerSignupCreate(
		Application application,
		Session     session,
		Hash        args)
{
	Credential   credential;
	User         user;
	char       * response_data;

	_controllerProcessUserCreateArgs(args, &user, &credential);

	if (0 == uuidCompare(
				uuidZero,
				applicationCreateUser(application, credential, user)))
	{
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
