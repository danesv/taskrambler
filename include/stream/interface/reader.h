/**
 * \file
 * Interface whose implementations can read from a stream given as
 * a handle.
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

#ifndef __STREAM_INTERFACE_READER_H__
#define __STREAM_INTERFACE_READER_H__

#include <sys/types.h>

#include "stream/stream.h"

typedef ssize_t (* fptr_streamReaderRead)(void *, Stream);

extern const struct interface i_StreamReader;

struct i_StreamReader {
	const struct interface * const _;
	fptr_streamReaderRead          read;
};

extern ssize_t streamReaderRead(void *, Stream);

#endif // __STREAM_INTERFACE_READER_H__

// vim: set ts=4 sw=4:
