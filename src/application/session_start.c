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


unsigned long
applicationSessionStart(Application this, const char * name, size_t nname)
{
	Session session = new(Session, name, nname);

	hashAdd(this->active_sessions, session);

	return session->id;
}

// vim: set ts=4 sw=4:
