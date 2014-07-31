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

#ifndef __AUTH_STORAGE_H__
#define __AUTH_STORAGE_H__

#include <sys/types.h>

#include <openssl/sha.h>

#include "trbase.h"
#include "storage/storage.h"


#define SALT_SIZE	32
#define HASH_SIZE	SHA512_DIGEST_LENGTH


TR_CLASS(AuthStorage) {
	Storage store;
};
TR_INSTANCE_INIT(AuthStorage);

/**
 * \todo In future this should use a more general purpose hash
 * function, which then will be in utils/hash.c
 */
int hash_pw(const char *, const size_t, unsigned char *, unsigned char **);

#endif // __AUTH_STORAGE_H__

// vim: set ts=4 sw=4:
