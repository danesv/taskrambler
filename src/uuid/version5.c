/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2013  Georg Hopp
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

// for size_t
#include <sys/types.h>

// for sha1 generation
#include <openssl/sha.h>

// for htonl and similar
#include <arpa/inet.h>

// for already available uuid functionality
#include "class.h"
#include "uuid.h"

void _uuidFormat3or5(Uuid uuid, unsigned char hash[16], int version);

Uuid
uuidVersion5(const unsigned char * name, size_t nname, Uuid nsid)
{
	SHA_CTX       ctx;
	unsigned char hash[20];
	Uuid          net_nsid = clone(nsid);
	Uuid          uuid     = new(Uuid);

	/*
	 * put the namespace id into network byte order.
	 */
	(net_nsid->uuid).elements.time_low =
		htonl((net_nsid->uuid).elements.time_low);
	(net_nsid->uuid).elements.time_mid =
		htons((net_nsid->uuid).elements.time_mid);
	(net_nsid->uuid).elements.time_hi_version =
		htons((net_nsid->uuid).elements.time_hi_version);

	/*
	 * generate the MD5
	 */
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, (net_nsid->uuid).value, 16);
	SHA1_Update(&ctx, name, nname);
	SHA1_Final(hash, &ctx);

	delete(net_nsid);

	_uuidFormat3or5(uuid, hash, 5);

	return uuid;
}

// vim: set ts=4 sw=4:
