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


User
userLoad(User this, Storage storage)
{
	char   * storage_data;
	size_t   nstorage_data;

	size_t * user_data_sizes;

	storageGet(
			storage,
			this->email, *this->nemail,
			&storage_data, &nstorage_data);

	if (NULL == storage_data) {
		return NULL;
	}

	user_data_sizes =
		(size_t *)(storage_data + nstorage_data - 3 * sizeof(size_t));

	this->nemail     = user_data_sizes;
	this->nfirstname = user_data_sizes + 1;
	this->nsurname   = user_data_sizes + 2;

	this->email     = storage_data;
	this->firstname = this->email + *this->nemail + 1;
	this->surname   = this->firstname + *this->nfirstname + 1;
	
	return this;
}

// vim: set ts=4 sw=4:
