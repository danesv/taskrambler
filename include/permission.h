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

#ifndef __PERMISSION_H__
#define __PERMISSION_H__

#include <sys/types.h>

#include "trbase.h"
#include "storage/storage.h"

/**
 * These might be hardcoded within the application.
 * In fact, as we are creating a restful CRUD application we might
 * consider not having these at all but define that each resource
 * might potentially have either Create, Read, Update and Delete
 * actions.
 *
 * Suggestion Actions as Bitmask...
 */
typedef enum e_ResourceActions {
	CREATE = 1,
	READ   = 2,
	UPDATE = 4,
	DELETE = 8
} ResourceAction;


TR_CLASS(Permission) {
	char             id[37];
	unsigned long    hash;

	char           * resource;
	ResourceAction   action;
};

#endif // __PERMISSION_H__

// vim: set ts=4 sw=4:

