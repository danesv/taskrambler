/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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
#include <stdint.h>
#include <sys/types.h>

#include "trbase.h"
#include "trdata.h"

#include "session.h"
#include "application/application.h"


static
inline
int
sessionIpIndexComp(const void * a, const void * b)
{
	Session sess_a = (Session)a;
	Session sess_b = (Session)b;

	if (sess_a->ip < sess_b->ip) {
		return -1;
	}

	if (sess_a->ip > sess_b->ip) {
		return 1;
	}

	return 0;
}

Session
applicationSessionStart(Application this, uint32_t ip)
{
	Session sess = TR_new(Session, ip);

	TR_hashAdd((this->active_sessions)[0].sessions, sess);
	TR_treeInsert(
			&((this->active_sessions)[0].ip_index),
			sess,
			sessionIpIndexComp);

	return sess;
}

// vim: set ts=4 sw=4:
