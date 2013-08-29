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


#include "class.h"
#include "asset.h"

#include "utils/mime_type.h"

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

	strncpy(this->fname, fname, 2047);
	this->fname[2048] = '\0';

	if (-1 == access(this->fname, O_RDONLY)) {
		return -1;
	} else {
		this->handle = open(this->fname, O_RDONLY);
		fstat(this->handle, &st);
	}

	tmp          = localtime(&(st.st_mtime));
	this->netag  = strftime(this->etag, sizeof(this->etag), "%s", tmp);
	this->nmtime = strftime(
			this->mtime, sizeof(this->mtime), "%a, %d %b %Y %T %Z", tmp);

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

	this->data = mmap(
			NULL, this->size, PROT_READ, MAP_PRIVATE, this->handle, 0);

	if (MAP_FAILED == this->data) {
		return -1;
	}

	this->ref_count = 1;

	return 0;
}

static void assetDtor(void * _this) {
	Asset this = _this;

	if (MAP_FAILED != this->data && NULL != this->data) {
		munmap(this->data, this->size);
	}

	if (0 != this->handle) {
		close(this->handle);
	}
}

INIT_IFACE(Class, assetCtor, assetDtor, NULL);
CREATE_CLASS(Asset, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
