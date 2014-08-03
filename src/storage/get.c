/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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
#include <stdlib.h>
#include <sys/types.h>

#include "trbase.h"
#include "storage/storage.h"


void
storageGet(
		Storage this,
		char * _key, size_t nkey,
		char ** data, size_t * ndata)
{
	char  * key      = TR_malloc(nkey);
	datum   gdbm_key = {key, nkey};
	datum   value;

	if (NULL == this) {
		*data  = NULL;
		*ndata = 0;
	}

	memcpy(key, _key, nkey);
	value = gdbm_fetch(this->gdbm, gdbm_key);
	TR_MEM_FREE(key);

	if (NULL != value.dptr) {
		*ndata = value.dsize;
		*data  = TR_malloc(value.dsize);
		memcpy(*data, value.dptr, value.dsize);

		free(value.dptr);
	} else {
		*data  = NULL;
		*ndata = 0;
	}
}

// vim: set ts=4 sw=4:
