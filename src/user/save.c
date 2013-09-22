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
#include <string.h>

#include "user.h"
#include "storage/storage.h"
#include "class.h"

#include "utils/memory.h"


void
userSave(User this, Storage storage)
{
	size_t storage_size =
		*this->nemail + 1 +
		*this->nfirstname + 1 +
		*this->nsurname + 1 +
		3 * sizeof(size_t);

	/*
	 * @TODO user return value for error handling 
	 */
	storageUpdate(
			storage,
			this->email, *this->nemail,
			this->email, storage_size);
}

// vim: set ts=4 sw=4:
