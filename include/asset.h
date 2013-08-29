/**
 * \file
 * Represents an asset (a file on disk)
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

#ifndef __ASSET_H__
#define __ASSET_H__

#include <sys/types.h>

#include "class.h"
#include "commons.h"
#include "hash.h"


CLASS(Asset) {
	char     fname[2048];
	char     etag[200];
	char     mtime[200];

	size_t   nfname;
	size_t   netag;
	size_t   nmtime;

	char   * mime_type;
	size_t   size;

	int      handle;
	char   * data;
};

char * assetDataAt(size_t);

#endif // __ASSET_H__

// vim: set ts=4 sw=4:
