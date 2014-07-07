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
#include <stdint.h>

#include <inttypes.h>
#include <stdio.h>

#include "trdata.h"

#include "session.h"
#include "application/application.h"

static
inline
	int
sessionIpIndexComp(const void * a, const void * b)
{
	Session  sess_a = (Session)a;
	uint32_t ip     = *(uint32_t *)b;

	if (sess_a->ip < ip) {
		return -1;
	}

	if (sess_a->ip > ip) {
		return 1;
	}

	return 0;
}

Session
applicationSessionGet(Application this, const char * sid, uint32_t ip)
{
	Session sess  = NULL;
	int     index;

	/**
	 * now get the session if not expired
	 */
	for (index=0; index<SESSION_LIVETIME; index++) {
		if (NULL != sid) {
			sess = (Session)TR_hashDelete(
					(this->active_sessions)[index].sessions, sid, 36);
		}

		if (NULL != sess) {
			/**
			 * update livetime of session if found
			 */
			sess->livetime = this->session_time_ofs + SESSION_LIVETIME;
			sess = (Session)TR_treeDelete(
					&((this->active_sessions)[index].ip_index),
					&ip, sessionIpIndexComp);
			TR_hashAdd((this->active_sessions)[0].sessions, sess);
			TR_treeInsert(
					&((this->active_sessions)[0].ip_index),
					sess,
					sessionIpIndexComp);
			break;
		} else {
			sess = (Session)TR_treeDelete(
					&((this->active_sessions)[index].ip_index),
					&ip, sessionIpIndexComp);
			if (NULL != sess) {
				// we have a previous session from this ip, remove it.
				TR_hashDelete(
						(this->active_sessions)[index].sessions,
						sess->id, 36);
				TR_delete(sess);
				break;
			}
		}
	}

	return sess;
}

// vim: set ts=4 sw=4:
