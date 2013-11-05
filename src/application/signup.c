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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "class.h"
#include "auth.h"
#include "user.h"
#include "uuid.h"
#include "storage/storage.h"
#include "application/application.h"

#include "interface/serializable.h"
#include "interface/indexable.h"

#include "utils/memory.h"
#include "commons.h"

int
applicationSignup(
		Application this,
		Credential  cred,
		User        user,
		Session     session)
{
	unsigned char   hash_data[SALT_SIZE+HASH_SIZE];
	unsigned char * salt = NULL;
	unsigned char * hash = hash_data+SALT_SIZE;
	char          * user_serialized;
	size_t          nuser_serialized;
	Uuid            index;

	index = indexUuid(user, this->user_namespace);
	serialize(user, (unsigned char **)&user_serialized, &nuser_serialized);

	if (FALSE == hash_pw(
				CRED_PWD(cred).pass,
				CRED_PWD(cred).npass,
				hash,
				&salt)) {
		/**
		 * \todo if we come here we have to delete the previously saved
		 * user again...
		 */
		return 0;
	}

	memcpy(hash_data, salt, SALT_SIZE);
	MEM_FREE(salt);

	/**
	 * \todo
	 * Add error handling here...
	 */
	storagePut(
			this->users,
			(char *)(index->uuid).value,
			sizeof((index->uuid).value),
			user_serialized,
			nuser_serialized);

	storagePut(
			this->passwords,
			(char *)(index->uuid).value,
			sizeof((index->uuid).value),
			(char *)hash_data,
			SALT_SIZE + HASH_SIZE);

	return 1;
}

// vim: set ts=4 sw=4:
