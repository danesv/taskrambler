/**
 * \file
 *
 * \author	Georg Hopp
 * \author	Unknown (find out)
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

#include <ctype.h>
#include <sys/types.h>

#include <openssl/rand.h>

#include "utils/hash.h"

/**
 * SDBM hashing algorithm:
 *
 * this algorithm was created for sdbm (a public-domain reimplementation of
 * ndbm) database library. it was found to do well in scrambling bits,
 * causing better distribution of the keys and fewer splits. it also happens
 * to be a good general hashing function with good distribution. the actual
 * function is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below
 * is the faster version used in gawk. [there is even a faster, duff-device
 * version] the magic constant 65599 was picked out of thin air while
 * experimenting with different constants, and turns out to be a prime. this
 * is one of the algorithms used in berkeley db (see sleepycat) and elsewhere. 
 */
unsigned long
sdbm(const unsigned char * str, size_t len)
{
	unsigned long hash = 0;

	for(; 0 < len; str++, len--)
		hash = tolower(*str) + (hash << 6) + (hash << 16) - hash;

	return hash;
}

// vim: set ts=4 sw=4:
