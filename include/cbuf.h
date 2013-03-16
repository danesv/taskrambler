/**
 * \file
 * My implementation of a ringbuffer.
 * It maps a shared memory object twice directly following
 * thus make it possible to read and write from any
 * position within the buffer without the nasty wrap
 * calculations.
 * This is achived because the same memory region is mapped
 * at the two addresses.
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

#ifndef __CBUF_H__
#define __CBUF_H__

#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#include "class.h"
#include "stream.h"

#include "commons.h"

#define ECBUFOVFL		100


CLASS(Cbuf) {
	char * shm_name;  // shared memory identifier

	char * data;
	Bool   lock;

	size_t bsize;
	size_t bused;

	size_t write;
	size_t read;
};

ssize_t cbufRead(Cbuf,  Stream);
ssize_t cbufWrite(Cbuf, Stream);

char *  cbufGetLine(Cbuf, char **);
char *  cbufGetData(Cbuf, size_t);
char *  cbufSetData(Cbuf, const void *, size_t);
void    cbufEmpty(Cbuf);

char *  cbufGetRead(Cbuf this);
char *  cbufGetWrite(Cbuf this);
char *  cbufMemchr(Cbuf this, int c);
size_t  cbufAddrIndex(Cbuf this, const void * c);
void    cbufIncRead(Cbuf this, size_t n);
void    cbufIncWrite(Cbuf this, size_t n);
size_t  cbufGetFree(Cbuf this);
char    cbufIsEmpty(Cbuf this);
void    cbufSkipNonAlpha(Cbuf this);
Bool    cbufIsLocked(Cbuf this);
void    cbufLock(Cbuf this);
void    cbufRelease(Cbuf this);

#endif // __CBUF_H__

// vim: set ts=4 sw=4:
