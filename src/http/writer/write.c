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

#include <trbase.h>

#include "http/message.h"
#include "trio.h"
#include "trdata.h"
#include "http/writer.h"


ssize_t
httpWriterWrite(void * _this, TR_Stream st)
{
	HttpWriter this = _this;

	switch (this->state) {
		char    * start;
		ssize_t   to_write;
		ssize_t   written;

		case HTTP_WRITER_GET:

		if (! TR_queueEmpty(this->queue)) {
			this->current = TR_queueGet(this->queue);

			this->written = 0;
			this->nheader = httpMessageHeaderSizeGet(this->current);

			if (this->nheader > TR_getSize(this->buffer)) {
				ssize_t size = this->nheader;

				size = (0 != size%WRITER_BUF_CHUNK)?
					(size/WRITER_BUF_CHUNK)+1 :
					size/WRITER_BUF_CHUNK;
				size *= WRITER_BUF_CHUNK;

				if (NULL != this->buffer) {
					TR_MEM_FREE(this->buffer);
				}

				this->buffer  = TR_malloc(size);
				this->nbuffer = size;
			}

			httpMessageHeaderToString(this->current, this->buffer);

			this->nbody = MIN(
					this->current->nbody,
					this->nbuffer - this->nheader);

			memcpy(
					this->buffer + this->nheader,
					this->current->body,
					this->nbody);

			this->state = HTTP_WRITER_WRITE;
		}
		else {
			break;
		}

		case HTTP_WRITER_WRITE:

		if (this->written >= this->nbuffer) {
			size_t body_done = this->written - this->nheader;

			start    = this->current->body + body_done;
			to_write = this->current->nbody - body_done;
		} else {
			start    = this->buffer   + this->written;
			to_write = (this->nheader + this->nbody) - this->written;
		}

		written = TR_streamWrite(st, start, to_write);

		if (written < 0) {
			return written;
		}

		this->written += written;

		if (written != to_write) {
			/*
			 * for some reason not all data could be
			 * written...most likely its a slow connection
			 * so, not to slow down the server we stop
			 * writing to this one now and come back to
			 * it in the next run....maybe it would be
			 * feasable to also implement some kind of
			 * timeout mechanism for writes...
			 * By the way, the same is true for reading,
			 * so to say, the parser.
			 */
			break;
		}

		if (this->written >= this->nheader + this->current->nbody) {
			// we are done with this message.
			this->state = HTTP_WRITER_DONE;
		} else {
			break;
		}

		case HTTP_WRITER_DONE:

		this->state = HTTP_WRITER_GET;

		if (! httpMessageHasKeepAlive(this->current)) {
			/**
			 * if the message did not have the keep-alive feature
			 * we don't care about further pipelined messages and
			 * return to the caller with a -2 indicating that the
			 * underlying connection should be closed at their side.
			 * Then we close to connection.
			 */
			TR_delete(this->current);
			return -2;
		}
		TR_delete(this->current);

		break;
	}

	return NULL == this->current ?
		this->queue->nmsg :
		this->queue->nmsg + 1;
}

// vim: set ts=4 sw=4:
