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
#include "uuid.h"
#include "storage/storage.h"

#include "interface/serializable.h"
#include "interface/indexable.h"

#include "utils/memory.h"
#include "application/application.h"


int
applicationLogin(
		Application this,
		Credential  credential,
		Session     session)
{
	Uuid       search;
	AuthModule auth_module;

	User user = new(User, NULL);

	user->username  = CRED_PWD(credential).user;
	user->nusername = &CRED_PWD(credential).nuser;
	search = indexUuid(user, this->user_namespace);

	auth_module = authenticate(this->auth, credential, search);

	if (0 != auth_module) {
		char   * user_serialized;
		size_t   nuser_serialized;

		session->user = user;

		switch (credential->type) {
			case CRED_PASSWORD:
				storageGet(
						this->users, 
						(char *)(search->uuid).value,
						sizeof((search->uuid).value),
						&user_serialized,
						&nuser_serialized);

				if (NULL != user_serialized) {
					unserialize(
							session->user,
							(unsigned char *)user_serialized,
							nuser_serialized);
					MEM_FREE(user_serialized);
				} else {
					/**
					 * this is a user authenticated via another method
					 * than the password database and has not yet
					 * logged in.
					 * NOTE: first we have to remove the search user and
					 * as username is initialized with something that we
					 * will free later here we must set it to NULL so that
					 * the delete will not free it.
					 */
					session->user->username = NULL;
					delete(session->user);
					session->user = new(User,
							CRED_PWD(credential).user,
							CRED_PWD(credential).nuser,
							CSTRA(""),
							CSTRA(""),
							CSTRA(""));

					serialize(
							session->user,
							(unsigned char **)&user_serialized,
							&nuser_serialized);
					/**
					 * \todo
					 * Handle error...if this fails we have most likely
					 * a collision.
					 */
					storagePut(
							this->users, 
							(char *)(search->uuid).value,
							sizeof((search->uuid).value),
							user_serialized,
							nuser_serialized);
					MEM_FREE(user_serialized);
				}

				session->user->auth_type = auth_module;
				break;

			default:
				break;
		}

		delete(search);
		return TRUE;
	}

	delete(search);

	return FALSE;
}

// vim: set ts=4 sw=4:
