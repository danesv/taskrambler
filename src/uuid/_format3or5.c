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

// for memcpy
#include <string.h>

// for ntohl and similar
#include <arpa/inet.h>

// for already available uuid functionality
#include <uuid/uuid.h>

#include "class.h"
#include "uuid.h"

void
_uuidFormat3or5(Uuid uuid, unsigned char hash[16], int version)
{
	/* convert UUID to local byte order */
	memcpy((uuid->uuid).value, hash, 16);

	(uuid->uuid).elements.time_low = 
		ntohl((uuid->uuid).elements.time_low);
	(uuid->uuid).elements.time_mid = 
		ntohs((uuid->uuid).elements.time_mid);
	(uuid->uuid).elements.time_hi_version =
		ntohs((uuid->uuid).elements.time_hi_version);

	/* put in the variant and version bits */
	(uuid->uuid).elements.time_hi_version  &= 0x0FFF;
	(uuid->uuid).elements.time_hi_version  |= (version << 12);
	(uuid->uuid).elements.clk_seq_hi_res   &= 0x3F;
	(uuid->uuid).elements.clk_seq_hi_res   |= 0x80;
}

// vim: set ts=4 sw=4:
