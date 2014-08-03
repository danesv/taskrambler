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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <trbase.h>
#include <trhash.h>

#include "auth.h"
#include "user.h"
#include "storage/storage.h"
#include "application/application.h"

TR_Uuid
applicationCreateUser(
		Application this,
		Credential  cred,
		User        user)
{
	char   * user_serialized;
	size_t   nuser_serialized;
	TR_Uuid  index;

	index = TR_getIndex(user);
	TR_serialize(user, (unsigned char **)&user_serialized, &nuser_serialized);

	if (SPR_OK != storagePut(
				this->users,
				(char *)(index->uuid).value,
				sizeof((index->uuid).value),
				user_serialized,
				nuser_serialized))
	{
		return TR_uuidZero;
	}

	TR_MEM_FREE(user_serialized);

	if (! applicationUpdatePassword(this, cred, user)) {
		/**
		 * \todo
		 * error handling is missing here
		 */
		storageDelete(
				this->users,
				(char *)(index->uuid).value,
				sizeof((index->uuid).value));

		return TR_uuidZero;
	}

	return index;
}

// vim: set ts=4 sw=4:
