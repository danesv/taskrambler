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

#include <sys/types.h>

#include "class.h"
#include "session.h"
#include "hash.h"
#include "application/application.h"

#include "utils/memory.h"


Session
applicationSessionGet(Application this, const char * sid)
{
	Session sess  = NULL;
	int     index;

	if (NULL != sid) {
		/**
		 * now get the session if not expired
		 */
		for (index=0; index<SESSION_LIVETIME; index++) {
			sess = (Session)hashDelete(
					(this->active_sessions)[index], sid, 36);
			if (NULL != sess) {
				break;
			}
		}

		/**
		 * update livetime of session if found
		 */
		if (NULL != sess) {
			sess->livetime = this->session_time_ofs + SESSION_LIVETIME;
			hashAdd((this->active_sessions)[0], sess);
		}
	}

	return sess;
}

// vim: set ts=4 sw=4:
