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

#include <stdarg.h>

// for mmap
#include <sys/mman.h>

// for open and fstat
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// for access
#include <unistd.h>

// for localtime
#include <time.h>


#include "trbase.h"
#include "asset.h"
#include "hash.h"

#include "utils/mime_type.h"
#include "utils/hash.h"
#include "utils/http.h"


static
int
assetCtor(void * _this, va_list * params)
{
	Asset this = _this;

    struct tm   * tmp;
    struct stat   st;
	char        * fname = va_arg(*params, char*);
	char        * ext;

	this->nfname = va_arg(*params, size_t);

	strncpy(this->fname, fname, 2048);
	this->fname[2048] = '\0';

	this->hash = sdbm(
			(unsigned char *)this->fname,
			this->nfname);

	if (-1 == access(this->fname, R_OK)) {
		return -1;
	} else {
		this->handle = open(this->fname, O_RDONLY);
		fstat(this->handle, &st);
	}

	tmp          = localtime(&(st.st_mtime));
	this->netag  = strftime(this->etag, sizeof(this->etag), "%s", tmp);
	this->nmtime = rfc1123Gmt(
			this->mtime,
			sizeof(this->mtime),
			&(st.st_mtime));

	this->size = st.st_size;

	ext = strrchr(this->fname, '.');
	if (NULL != ext) {
		ext++;
		this->mime_type = getMimeType(ext, strlen(ext));
	} else {
		this->mime_type = "application/octet-stream";
	}

	if (NULL != this->mime_type) {
		this->nmime_type = strlen(this->mime_type);
	} else {
		this->nmime_type = 0;
	}

	if (0 < this->size) {
		this->data = mmap(
				NULL, this->size, PROT_READ, MAP_PRIVATE, this->handle, 0);

		if (MAP_FAILED == this->data) {
			close(this->handle);
			return -1;
		}
	}

	this->ref_count = 1;

	return 0;
}

static void assetDtor(void * _this) {
	Asset this = _this;

	if (MAP_FAILED != this->data && NULL != this->data) {
		munmap(this->data, this->size);
	}

	if (0 < this->handle) {
		close(this->handle);
	}
}

static
unsigned long
assetGetHash(void * _this)
{
	Asset this = _this;

	return this->hash;
}

static
void
assetHandleDouble(void * _this, void * _doub)
{
}

TR_INIT_IFACE(TR_Class, assetCtor, assetDtor, NULL);
TR_INIT_IFACE(Hashable, assetGetHash, assetHandleDouble);
TR_CREATE_CLASS(Asset, NULL, TR_IF(TR_Class), TR_IF(Hashable));

// vim: set ts=4 sw=4:
