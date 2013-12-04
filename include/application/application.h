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

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <sys/types.h>

#include "trbase.h"

#include "session.h"
#include "hash.h"
#include "auth.h"
#include "auth/credential.h"
#include "storage/storage.h"
#include "session.h"
#include "user.h"
#include "uuid.h"


struct randval {
	time_t timestamp;
	int    value;
};

TR_CLASS(Application) {
	Hash            * active_sessions;
	time_t            session_time_ofs;

	Auth              auth;

	struct randval  * val;

	Storage           users;
	Storage           passwords;
	Storage           roles;

	Uuid              user_namespace;

	Hash              roles_user_index;
	Hash              roles_resource_index;

	const char *      version;
	const char *      loc;
};

int  applicationLogin(Application, Credential, Session);
void applicationLogout(Application, Session);
Uuid applicationCreateUser(Application, Credential, User);
Uuid applicationUpdateUser(Application, User);
User applicationGetUser(Application, Uuid);
int  applicationUpdatePassword(Application, Credential, User);

Session applicationSessionStart(Application);
Session applicationSessionGet(Application, const char *);
void    applicationSessionStop(Application, Session);
void    applicationSessionCleanup(Application, time_t);

#endif // __HTTP_HEADER_H__

// vim: set ts=4 sw=4:
