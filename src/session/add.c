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

#include <search.h>

#include "session.h"
#include "class.h"


static
inline
int
sessionAddComp(const void * _a, const void * _b)
{
	Session a = (Session)_a;
	Session b = (Session)_b;
	return (a->id < b->id)? -1 : (a->id > b->id)? 1 : 0;
}

Session
sessionAdd(const Session * root, Session session)
{
	Session * found = tsearch(session, (void**)root, sessionAddComp);

	if (NULL == found) {
		return NULL;
	}

	if (*found != session) {
		/**
		 * \todo this should not happen, so do some logging here.
		 */
		delete(session);
	}

	return *found;
}

// vim: set ts=4 sw=4:
