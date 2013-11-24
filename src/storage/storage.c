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
#include <sys/stat.h>

#include "class.h"
#include "storage/storage.h"

#include "utils/memory.h"

static
int
storageCtor(void * _this, va_list * params)
{
	Storage   this    = _this;
	char    * db_name = va_arg(* params, void *);

	this->db_name = memMalloc(strlen(db_name) + 1);
	strcpy(this->db_name, db_name);

	this->gdbm = gdbm_open(
			this->db_name,
			0,
			GDBM_WRCREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
			NULL);

	if (NULL == this->gdbm) {
		return -1;
	}

	return 0;
}

static
void
storageDtor(void * _this)
{
	Storage this = _this;

	if (NULL != this->db_name) MEM_FREE(this->db_name);
	if (NULL != this->gdbm) {
		gdbm_close(this->gdbm);
		this->gdbm = NULL;
	}
}

INIT_IFACE(Class, storageCtor, storageDtor, NULL);
CREATE_CLASS(Storage, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
