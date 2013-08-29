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

#include "class.h"
#include "http/message.h"
#include "queue.h"
#include "http/writer.h"
#include "cbuf.h"
#include "stream.h"

#include "commons.h"


ssize_t
httpWriterWrite(void * _this, Stream st)
{
	HttpWriter this = _this;
	int        cont = 1;

	if (cbufIsLocked(this->buffer)) {
		if (FALSE == this->ourLock)
			return 0;
	}
	else {
		cbufLock(this->buffer);
		this->ourLock = TRUE;
	}

	while (cont) {
		switch (this->state) {
			case HTTP_WRITER_GET:
				if (NULL == this->current && ! queueEmpty(this->queue)) {
					this->current = queueGet(this->queue);

					this->written = 0;
					this->nbody   = 0;
					this->nheader = httpMessageHeaderSizeGet(this->current);

					httpMessageHeaderToString(
							this->current,
							cbufGetWrite(this->buffer));
					cbufIncWrite(this->buffer, this->nheader);

					this->state = HTTP_WRITER_WRITE;
				}
				else {
					cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont          = 0;
				}
				break;

			case HTTP_WRITER_WRITE:
				/**
				 * read
				 */
				if (this->nbody < this->current->nbody) {
					size_t size = MIN(
							this->current->nbody - this->nbody,
							cbufGetFree(this->buffer));

					cbufSetData(this->buffer,
							this->current->body + this->nbody,
							size);

					this->nbody += size;
				}

				/**
				 * write
				 */
				{
					ssize_t written = cbufWrite(this->buffer, st);

					if (0 > written) {
						return written;
					}

					this->written += written;
				}

				if (this->written == this->current->nbody + this->nheader) {
					this->state = HTTP_WRITER_DONE;
				}
				else {
					cont = 0;
				}
				break;

			case HTTP_WRITER_DONE:
	 			this->state = HTTP_WRITER_GET;

				cbufRelease(this->buffer);
				this->ourLock = FALSE;

				if (! httpMessageHasKeepAlive(this->current)) {
					/**
					 * if the message did not have the keep-alive feature
					 * we don't care about further pipelined messages and
					 * return to the caller with a -1 indicating that the
					 * underlying connection should be closed at their side.
					 * Then we close to connection.
					 */
					delete(this->current);
					return -1;
				}
				delete(this->current);

				break;
		}
	}

	return this->queue->nmsg;
}

// vim: set ts=4 sw=4:
