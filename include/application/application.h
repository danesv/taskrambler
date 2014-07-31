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
#include "trhash.h"
#include "trdata.h"

#include "session.h"
#include "auth.h"
#include "auth/credential.h"
#include "storage/storage.h"
#include "session.h"
#include "user.h"


struct randval {
	time_t timestamp;
	int    value;
};

struct sessinfo {
	TR_Hash sessions;
	TR_Tree ip_index;
};

TR_CLASS(Application) {
	struct sessinfo * active_sessions;
	time_t            session_time_ofs;

	Auth              auth;

	struct randval  * val;

	Storage           users;
	Storage           passwords;
	Storage           roles;

	TR_Uuid           user_namespace;

	TR_Hash           roles_user_index;
	TR_Hash           roles_resource_index;

	const char      * version;
	const char      * loc;
};
TR_INSTANCE_INIT(Application);

int     applicationLogin(Application, Credential, Session);
void    applicationLogout(Application, Session);
TR_Uuid applicationCreateUser(Application, Credential, User);
TR_Uuid applicationUpdateUser(Application, User);
User    applicationGetUser(Application, TR_Uuid);
int     applicationUpdatePassword(Application, Credential, User);

Session applicationSessionStart(Application, uint32_t);
Session applicationSessionGet(Application, const char *, uint32_t);
void    applicationSessionStop(Application, Session);
void    applicationSessionCleanup(Application, time_t);

#endif // __HTTP_HEADER_H__

// vim: set ts=4 sw=4:
