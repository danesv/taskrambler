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

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

// for fopen
#include <stdio.h>

#include "trbase.h"
#include "trdata.h"

TR_Hash mime_types = NULL;

void
readMimeTypes(void)
{
	if (0 == access(CONFIGDIR "/mime.types", O_RDONLY)) {
		FILE * handle = fopen(CONFIGDIR "/mime.types", "r");

		if (NULL != handle) {
			char buffer[512];

			buffer[511] = '\0';
			mime_types  = TR_new(TR_Hash);

			while (NULL != fgets(buffer, 511, handle)) {
				char * tmp;
				char * key = buffer;
				char * value;
				size_t nkey;
				size_t nvalue;

				tmp = memchr(key, ' ', 512);

				if (NULL != tmp) {
					*tmp = '\0';
				}
				nkey = tmp - buffer;

				value = tmp + 1;
				for (; *value == ' ' && value < buffer+511; value++);

				nvalue = strlen(value);

				if ('\n' == value[nvalue-1]) {
					nvalue--;
					value[nvalue] = '\0';
				}   

				TR_hashAdd(mime_types,
						TR_new(TR_HashValue, key, nkey, value, nvalue));
			}           

			fclose(handle);
		}   
	}
}

char *
getMimeType(const char * ext, size_t len)
{
	TR_HashValue type;

	if (NULL == mime_types) {
		readMimeTypes();

		if (NULL == mime_types) {
			return NULL;
		}
	}

	type = TR_hashGet(mime_types, ext, len);

	if (NULL == type) {
		return NULL;
	}

	return (char *)type->value;
}

void
clearMimeTypes(void)
{
	TR_delete(mime_types);
}

// vim: set ts=4 sw=4:
