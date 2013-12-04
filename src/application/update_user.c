/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2013  Georg Hopp
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "trbase.h"
#include "auth.h"
#include "user.h"
#include "uuid.h"
#include "storage/storage.h"
#include "application/application.h"


Uuid
applicationUpdateUser(
		Application this,
		User        user)
{
	char   * user_serialized;
	size_t   nuser_serialized;
	Uuid     index;

	index = TR_indexUuid(user, this->user_namespace);
	TR_serialize(user, (unsigned char **)&user_serialized, &nuser_serialized);

	if (SPR_OK != storageUpdate(
				this->users,
				(char *)(index->uuid).value,
				sizeof((index->uuid).value),
				user_serialized,
				nuser_serialized))
	{
		return uuidZero;
	}

	TR_MEM_FREE(user_serialized);

	return index;
}

// vim: set ts=4 sw=4:
