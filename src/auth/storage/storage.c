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

#include "class.h"
#include "storage.h"
#include "auth.h"
#include "commons.h"
#include "utils/memory.h"

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
authStorageAuthenticate(void * _this, Credential cred)
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
			CRED_PWD(cred).user,
			CRED_PWD(cred).nuser,
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
		MEM_FREE(found_hash);
		return FALSE;
	}

	if (0 != memcmp(current_hash, found_hash+SALT_SIZE, HASH_SIZE)) {
		MEM_FREE(found_hash);
		return FALSE;
	}

	MEM_FREE(found_hash);
	return TRUE;
}

INIT_IFACE(Class, authStorageCtor, authStorageDtor, NULL);
INIT_IFACE(Auth, authStorageAuthenticate);
CREATE_CLASS(AuthLdap, NULL, IFACE(Class), IFACE(Auth));

// vim: set ts=4 sw=4:
