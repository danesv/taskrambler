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
	Session sess    = NULL;
	Queue   current = this->active_sessions;
	time_t  now     = time(NULL);

	while (NULL != current->next) {
		Session session = (Session)current->next->msg;

		if (now >= session->livetime) {
			Queue to_delete = current->next;

			if (to_delete == this->active_sessions->first) {
				this->active_sessions->first = to_delete->next;
			}
			if (to_delete == this->active_sessions->last) {
				if (to_delete == this->active_sessions->next) {
					this->active_sessions->last = NULL;
				} else {
					this->active_sessions->last = current;
				}
			}

			current->next = to_delete->next;

			delete(session);
			delete(to_delete);
			continue;
		}

		if (NULL != sid && 0 == memcmp(session->id, sid, 36)) {
			session->livetime = time(NULL) + SESSION_LIVETIME;
			sess              = session;
		}

		current = current->next;
	}

	return sess;
}

// vim: set ts=4 sw=4:
