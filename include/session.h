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

#ifndef __SESSION_H__
#define __SESSION_H__

#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include <user.h>

#include "trbase.h"

// livetime of a session in seconds
#define SESSION_LIVETIME	300 // 5 minutes

/**
 * Having only a session livetime is not enough.
 * An attacker might create a client that never sends a session id
 * back and continuously sends requests. This will then result
 * in newly created sessions.
 * The session class uses 57 bytes
 * But there is also a user object created all the time.
 * This uses 80 bytes.
 * Each user in turn contains a uuid which is 37 bytes.
 * Each of these are a class which adds another 221 bytes to each.
 * So the following is allocated for these three objects:
 * Session: 57 + 221 = 278
 * User:    80 + 221 = 301
 * Uuid:    37 + 221 = 258
 * My allocater only allocates power of 2 sizes to optimize
 * memory management so we end up with 512 bytes per object which is
 * 1536 bytes per created session.
 * The current code is able to handle more than 25000 request per 
 * second if there is no session id provided on my hardware.
 * This sums up to 10GB of used memory within the 5 minutes
 * session livetime.
 *
 * To prevent this I associate the session with the ip it was
 * created on. If there then is a subsequent request from the same ip
 * without a sessionid, the old session can be removed and a new one
 * can be created. This might give a small but acceptable performance
 * hit.
 */

TR_CLASS(Session) {
	char          id[37];
	unsigned long hash;
	uint32_t      ip;

	time_t        livetime;
	User          user;
};
TR_INSTANCE_INIT(Session);
TR_CLASSVARS_DECL(Session) {};

#endif // __SESSION_H__

// vim: set ts=4 sw=4:
