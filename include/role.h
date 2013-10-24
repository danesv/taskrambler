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

#ifndef __ROLE_H__
#define __ROLE_H__

#include <sys/types.h>

#include "class.h"
#include "ebac/permission.h"
#include "storage/storage.h"


CLASS(Role) {
	char            id[37];
	unsigned long   hash;

	char          * name;
	size_t          nname;

	/**
	 * \todo We need a good way to serialize a hash.
	 * If I can't find any I should choose a different
	 * data structure here...but I think there is a way.
	 */
	Hash            permissions;
	Hash            users;
};

#endif // __ROLE_H__

// vim: set ts=4 sw=4:

