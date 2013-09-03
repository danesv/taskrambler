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

// for size_t
#include <sys/types.h>

// for strlen

#include "class.h"
#include "asset.h"
#include "hash.h"

Hash asset_pool = NULL;

static
inline
void
freeAsset(const void * _node)
{
	delete(_node);
}

Asset
assetPoolGet(const char * path, size_t npath)
{
	Asset asset = NULL;

	if (NULL == asset_pool) {
		asset_pool = new(Hash);
	} else {
		asset = hashGet(asset_pool, path, npath);
	}

	if (NULL == asset) {
		asset = new(Asset, path, npath);
		hashAdd(asset_pool, asset);
	} else {
		asset->ref_count++;
	}

	return asset;
}

size_t
assetPoolRelease(Asset asset)
{
	if (asset->ref_count <= 1) {
		hashDelete( asset_pool, asset->fname, asset->nfname);

		if (NULL != asset) {
			delete(asset);
		}

		return 0;
	} else {
		asset->ref_count--;

		return asset->ref_count;
	}
}

void
assetPoolCleanup(void)
{
	if (NULL != asset_pool) {
		hashEach(asset_pool, freeAsset);
		delete(asset_pool);
	}
}

// vim: set ts=4 sw=4:
