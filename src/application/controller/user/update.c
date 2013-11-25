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
#include "user.h"

#include "utils/memory.h"
#include "commons.h"

char * controllerCurrentuserRead(Application, Session, Hash);
int    _controllerUpdateUserFromArgs(Hash, User *);

char *
controllerUserUpdate(
		Application application,
		Session     session,
		Hash        args)
{
	Uuid user_id;

	if (! _controllerUpdateUserFromArgs(args, &(session->user))) {
		return NULL;
	}

	user_id = applicationUpdateUser(application, session->user);
	if (0 == uuidCompare(uuidZero, user_id)) {
		return NULL;
	}
	delete(user_id);

	return controllerCurrentuserRead(application, session, NULL);
}

// vim: set ts=4 sw=4:
