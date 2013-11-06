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

#include <gdbm.h>
#include <string.h>
#include <sys/types.h>

#include "class.h"
#include "storage/storage.h"

#include "utils/memory.h"
#include "commons.h"

int
storageDelete(Storage this, char * _key, size_t nkey)
{
	datum key = {_key, nkey};

	switch (gdbm_delete(this->gdbm, key)) {
		case 0:
			return TRUE;
		case -1:
		default:
			return FALSE;
	}

	return FALSE;
}

// vim: set ts=4 sw=4:
