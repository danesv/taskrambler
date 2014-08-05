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

// for size_t
#include <sys/types.h>

// for strlen

#include "trbase.h"
#include "trdata.h"
#include "asset.h"

TR_Hash asset_pool = NULL;

static
inline
void
freeAsset(const void * _node, const void * data)
{
	TR_delete(_node);
}

Asset
assetPoolGet(const char * path, size_t npath)
{
	Asset asset = NULL;

	if (NULL == asset_pool) {
		asset_pool = TR_new(TR_Hash);
	} else {
		asset = TR_hashGet(asset_pool, path, npath);
	}

	if (NULL == asset) {
		asset = TR_new(Asset, path, npath);
		if (NULL != asset) {
			TR_hashAdd(asset_pool, asset);
		}
	} else {
		asset->ref_count++;
	}

	return asset;
}

size_t
assetPoolRelease(Asset asset)
{
	if (asset->ref_count > 1) {
		asset->ref_count--;
		return asset->ref_count;
	}

	if (NULL != asset) {
		Asset found = (Asset)TR_hashDelete(
				asset_pool, asset->fname, asset->nfname);

		if (found == asset) {
			TR_delete(found);
		} else {
			/**
			 * \todo this should never happen....error log...
			 */
		}
	}

	return 0;
}

void
assetPoolCleanup(void)
{
	if (NULL != asset_pool) {
		TR_hashEach(asset_pool, freeAsset, NULL);
		TR_delete(asset_pool);
	}
}

// vim: set ts=4 sw=4:
