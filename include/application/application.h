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
#include "class.h"

#include "session.h"
#include "queue.h"
#include "auth/credential.h"
#include "storage.h"
#include "session.h"
#include "user.h"


struct randval {
	time_t timestamp;
	int    value;
};

CLASS(Application) {
	// should be a list and not a queue but currently queue is 
	// the closest I have.
	Queue             active_sessions;

	void           ** auth;
	size_t            nauth;
	struct randval  * val;

	Storage           users;
	Storage           passwords;
};

int     applicationLogin(Application, Credential, Session);
int     applicationSignup(Application, Credential, User, Session);

Session applicationSessionStart(Application);
Session applicationSessionGet(Application, const char *);
void    applicationSessionStop(Application, const char *);
void    applicationSessionUpdate(
		Application, const char *, const char *, size_t);

#endif // __HTTP_HEADER_H__

// vim: set ts=4 sw=4:
