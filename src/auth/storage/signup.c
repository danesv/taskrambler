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

#include <sys/types.h>

#include "class.h"
#include "storage.h"
#include "auth/storage.h"
#include "auth/credential.h"

int
authStorageSignup(AuthStorage this, Credential cred)
{
	unsigned char hash[SALT_SIZE+HASH_SIZE];

	if (FALSE == hash_pw(
				CRED_PWD(cred).pass,
				CRED_PWD(cred).npass,
				&hash,
				&(hash+SALT_SIZE))) {
		return 0;
	}

	storagePut(
			this->store,
			CRED_PWD(cred).user,
			CRED_PWD(cred).nuser,
			hash,
			SALT_SIZE + HASH_SIZE);

	return 1;
}

// vim: set ts=4 sw=4:
