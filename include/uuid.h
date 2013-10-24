/**
 * \file
 * ways to create uuid variant 5. For uuid of variant 1 I use
 * the implementation delivered with the core utils.
 * But this is wrapped in here, so that the rest of the code
 * can use only this implementation...this additionally has the
 * advantage that we can implement version 1 here too for systems
 * where the coreutils implementation is not available.
 *
 * \author  Georg Hopp
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

#ifndef __UUID_H__
#define __UUID_H__

#include <stdint.h>
#include <sys/types.h>
#include <uuid/uuid.h>

#include "class.h"
#include "commons.h"


typedef char UuidString[37];

CLASS(Uuid) {
	union {
		uuid_t value;
		struct {
			uint32_t      time_low;
			uint16_t      time_mid;
			uint16_t      time_hi_version;
			uint8_t       clk_seq_hi_res;
			uint8_t       clk_seq_low;
			unsigned char node[6];
		} elements;
	} uuid;
};

/*
 * generator functions...these are not really part of the object
 * but generate a uuid object.
 */
Uuid uuidVersion1();
Uuid uuidVersion3(const unsigned char *, size_t, Uuid);
Uuid uuidVersion5(const unsigned char *, size_t, Uuid);

void uuidUnparse(Uuid, UuidString);
Uuid uuidParse(const UuidString);

int uuidCompare(Uuid, Uuid);

#endif // __UUID_H__

// vim: set ts=4 sw=4:
