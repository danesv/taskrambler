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

#define _GNU_SOURCE

#include <string.h>
#include <sys/types.h>

#include "class.h"
#include "session.h"
#include "hash.h"
#include "application/application.h"

#include "utils/memory.h"


void
applicationSessionUpdate(
		Application   this,
		const char *  sid,
		const char *  name,
		size_t        nname)
{
//	Session session = hashGet(this->active_sessions, sid, 36);
//
//	if (NULL != session) {
//		MEM_FREE(session->username);
//
//		session->username        = memMalloc(nname + 1);
//		session->username[nname] = 0;
//		memcpy(session->username, name, nname);
//	}
}

// vim: set ts=4 sw=4:
