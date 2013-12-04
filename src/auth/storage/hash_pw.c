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

#include <openssl/evp.h>
#include <openssl/rand.h>

#include "trbase.h"
#include "auth/storage.h"

/*
 * I have to hash the passwords, maybe this will move in
 * a separate class in future, but now everything is done
 * here
 */
#define PBKDF2_ITERATIONS   2048

/*
 * base64 decode via openssl...
 * I do not need this i think, but I keep it...maybe I have
 * use for it later.
 *
#include <openssl/bio.h>
#include <openssl/evp.h>

#define B64_SALT  "q36MilkD6Ezlt6+G394aPYWrSwAdEhdnK8k="

BIO_METHOD * BIO_f_base64(void);

void
base64decode(char * data) {
	BIO         * bio,
		        * b64;
	FILE        * b64_salt = fmemopen(B64_SALT, sizeof(B64_SALT)-1, "r");

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(b64_salt, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

	if (SALT_SIZE != BIO_read(bio, data, SALT_SIZE)) {
		return -1;
	}

	BIO_free_all(bio);
	fclose(b64_salt);
}
*/

int
hash_pw(
		const char    *  password,
		const size_t     npassword,
		unsigned char *  hash,
		unsigned char ** salt)
{
	if (NULL == *salt) {
		*salt = TR_calloc(SALT_SIZE, sizeof(unsigned char));
		if (0 > RAND_pseudo_bytes(*salt, SALT_SIZE)) {
			TR_MEM_FREE(*salt);
			return FALSE;
		}
	}

	if (0 == PKCS5_PBKDF2_HMAC(
			password,
			npassword,
			*salt,
			SALT_SIZE,
			PBKDF2_ITERATIONS,
			EVP_sha512(),
			HASH_SIZE,
			hash)) {
		TR_MEM_FREE(*salt);
		return FALSE;
	}

	return TRUE;
}

// vim: set ts=4 sw=4:
