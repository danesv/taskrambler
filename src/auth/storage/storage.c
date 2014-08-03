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

#include <trbase.h>
#include <trhash.h>

#include "storage/storage.h"
#include "auth.h"
#include "user.h"

static
int
authStorageCtor(void * _this, va_list * params)
{
	AuthStorage   this = _this;

	this->store = va_arg(*params, Storage);

	return 0;
}

static
void
authStorageDtor(void * _this)
{
}

static
int
authStorageAuthenticate(void * _this, Credential cred, TR_Uuid user_index)
{
	AuthStorage this = _this;

	unsigned char   current_hash[HASH_SIZE];
	unsigned char * found_hash  = NULL;
	size_t          nfound_hash = 0;

	if (CRED_PASSWORD != cred->type) {
		return FALSE;
	}

	storageGet(
			this->store,
			(char *)(user_index->uuid).value,
			sizeof((user_index->uuid).value),
			(char **)&found_hash,
			&nfound_hash);

	if (NULL == found_hash || (SALT_SIZE + HASH_SIZE) != nfound_hash) {
		/* user not found or found hash is invalid */
		return FALSE;
	}

	/* found_hash <=> salt+hash */
	if (FALSE == hash_pw(
				CRED_PWD(cred).pass,
				CRED_PWD(cred).npass,
				current_hash,
				&found_hash)) {
		TR_MEM_FREE(found_hash);
		return FALSE;
	}

	if (0 != memcmp(current_hash, found_hash+SALT_SIZE, HASH_SIZE)) {
		TR_MEM_FREE(found_hash);
		return FALSE;
	}

	TR_MEM_FREE(found_hash);
	return TRUE;
}

TR_INIT_IFACE(TR_Class, authStorageCtor, authStorageDtor, NULL);
TR_INIT_IFACE(Auth, authStorageAuthenticate);
TR_CREATE_CLASS(AuthStorage, NULL, NULL, TR_IF(TR_Class), TR_IF(Auth));

// vim: set ts=4 sw=4:
