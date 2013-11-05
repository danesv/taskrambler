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

#include <stdlib.h>
#include <sys/types.h>

#include "class.h"
#include "session.h"
#include "hash.h"
#include "application/application.h"

#include "utils/memory.h"

void
applicationSessionCleanup(Application this, time_t now)
{
	unsigned int expired = now - this->session_time_ofs;
	unsigned int i       = 0;

	if (SESSION_LIVETIME <= expired) {
		expired = SESSION_LIVETIME;
	}

	if (0 < expired && SESSION_LIVETIME > expired) {
		Hash * tmp_buf = memCalloc(SESSION_LIVETIME, sizeof(Hash));

		memcpy(
				&(tmp_buf[expired]),
				this->active_sessions,
				(SESSION_LIVETIME - expired) * sizeof(Hash));
		memcpy(
				tmp_buf,
				&(this->active_sessions[SESSION_LIVETIME - expired]),
				expired * sizeof(Hash));
				
		MEM_FREE(this->active_sessions);
		this->active_sessions = tmp_buf;
	}

	for (i=0; i<expired; i++) {
		hashCleanup(this->active_sessions[i]);
	}

	this->session_time_ofs = now;
}

// vim: set ts=4 sw=4:
