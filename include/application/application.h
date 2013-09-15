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
#include "hash.h"
#include "auth/credential.h"

struct randval {
	time_t timestamp;
	int    value;
};

CLASS(Application) {
	Hash             active_sessions;
	void           * auth;
	struct randval * val;
};

// this should return a user account....now it only return success or failure.
int           applicationLogin(Application, Credential);
unsigned long applicationSessionStart(Application, const char *, size_t);
void          applicationSessionStop(Application, unsigned long);
void          applicationSessionUpdate(
		Application, unsigned long, const char *, size_t);
Session       applicationSessionGet(Application, unsigned long);

#endif // __HTTP_HEADER_H__

// vim: set ts=4 sw=4: