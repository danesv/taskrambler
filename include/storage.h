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

#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <gdbm.h>
#include <sys/types.h>

#include "class.h"


CLASS(Storage) {
	GDBM_FILE   gdbm;
	char      * db_name;
};

void storagePut(Storage, char *, size_t, char *, size_t);
void storageGet(Storage, char *, size_t, char **, size_t *);

#endif // __STORAGE_H__

// vim: set ts=4 sw=4:

