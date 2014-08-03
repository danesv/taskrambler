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
#include "storage/storage.h"
#include "application/application.h"


int
applicationLogin(
		Application this,
		Credential  credential,
		Session     session)
{
	TR_Uuid    search;
	AuthModule auth_module;

	User user = TR_new(User, this->user_namespace, NULL);

	user->username  = CRED_PWD(credential).user;
	user->nusername = &CRED_PWD(credential).nuser;
	search = TR_getIndex(user);

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
					TR_unserialize(
							session->user,
							(unsigned char *)user_serialized,
							nuser_serialized);
					TR_MEM_FREE(user_serialized);
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
					TR_delete(session->user);
					session->user = TR_new(User,
							this->user_namespace,
							CRED_PWD(credential).user,
							CRED_PWD(credential).nuser,
							CSTRA(""),
							CSTRA(""),
							CSTRA(""));

					TR_serialize(
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
					TR_MEM_FREE(user_serialized);
				}

				session->user->auth_type = auth_module;
				break;

			default:
				break;
		}

		TR_delete(search);
		return TRUE;
	}

	TR_delete(search);

	return FALSE;
}

// vim: set ts=4 sw=4:
