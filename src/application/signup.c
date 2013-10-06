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
#include "application/application.h"

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

	if (NULL != userLoad(user, this->users)) {
		/*
		 * if any user is found with this email return false
		 * as on signup equal email adresses are not allowed
		 * at all.
		 */
		return 0;
	}

	userSave(user, this->users);

	if (FALSE == hash_pw(
				CRED_PWD(cred).pass,
				CRED_PWD(cred).npass,
				hash,
				&salt)) {
		/*
		 * @TODO if we come here we have to delete the previously saved
		 * user again...
		 */
		return 0;
	}

	memcpy(hash_data, salt, SALT_SIZE);

	MEM_FREE(salt);

	storagePut(
			this->passwords,
			CRED_PWD(cred).user,
			CRED_PWD(cred).nuser,
			(char *)hash_data,
			SALT_SIZE + HASH_SIZE);

	return 1;
}

// vim: set ts=4 sw=4:
