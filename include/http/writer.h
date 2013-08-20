/**
 * \file
 * Write ready responses to an output stream.
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

#ifndef __HTTP_WRITER_H__
#define __HTTP_WRITER_H__

#include <sys/types.h>

#include "class.h"
#include "http/message.h"
#include "queue.h"
#include "cbuf.h"
#include "stream.h"

#include "commons.h"


/*
 * the buffer that will be written back to an http client.
 * If we have open 1024 paralell connection this will result
 * in a memory usage of 128MB. Right now, we don't allow more
 * than this amount of paralell connections.
 * 
 * This one and the parser buffer are the hugest memory pools
 * we need. The parser buffer is of the same size.
 *
 * Right now only the ringbuffer is reused for each connection
 * resulting in some memory movement between some temporary
 * space and the circular buffer.
 *
 * This behavioru should be kept in place for low memory machines
 * running this code.
 *
 * Anyway, I will build a version which uses two ringbuffers for
 * each connection, Resulting in a 256KB memory used for each
 * connection. Which in turn means 256MB for 1024 paralell connections.
 *
 * And as I will also implement a cbuf pool, this memory will not be
 * freed before application end.
 */
#define WRITER_MAX_BUF	131072


typedef enum e_HttpWriterState {
	HTTP_WRITER_GET=0,
	HTTP_WRITER_WRITE,
	HTTP_WRITER_DONE
} HttpWriterState;

CLASS(HttpWriter) {
	Cbuf        buffer;
	Bool        ourLock;

	Queue       queue;
	HttpMessage current;

	size_t      nheader;
	size_t      nbody;
	size_t      written;

	HttpWriterState state;
};

ssize_t httpWriterWrite(void *, Stream);

#endif // __HTTP_WRITER_H__

// vim: set ts=4 sw=4:
