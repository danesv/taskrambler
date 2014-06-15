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

#include <stdlib.h>

#include "trbase.h"
#include "trio.h"
#include "trdata.h"

#include "http/parser.h"
#include "http/header.h"
#include "http/interface/http_intro.h"


ssize_t
httpParserParse(void * _this, TR_Stream st)
{
	HttpParser this = _this;
	int        cont = 1;
	ssize_t    read;
	char *     line;
	char *     line_end;

	if (TR_cbufIsLocked(this->buffer)) {
		if (FALSE == this->ourLock)
			return 0;
	}
	else {
		TR_cbufLock(this->buffer);
		this->ourLock = TRUE;
	}

	if (NULL != this->incomplete) {
		TR_cbufSetData(this->buffer, this->incomplete, this->isize);
		TR_MEM_FREE(this->incomplete);
	}

	if (0 > (read = TR_cbufRead(this->buffer, st))) {
		TR_cbufRelease(this->buffer);
		this->ourLock = FALSE;
		return read;
	}

	while (cont) {
		switch(this->state) {
			case HTTP_MESSAGE_GARBAGE:
				TR_cbufSkipNonAlpha(this->buffer);
				if (! TR_cbufIsEmpty(this->buffer)) {
					this->state = HTTP_MESSAGE_START;
				}
				else {
					TR_cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont          = 0;
					break;
				}

			case HTTP_MESSAGE_START:
				if (NULL == (line = TR_cbufGetLine(this->buffer, &line_end))) {
					if (! TR_cbufIsEmpty(this->buffer)) {
						this->isize      = this->buffer->bused;
						this->incomplete = TR_malloc(this->isize);
						memcpy(this->incomplete,
								TR_cbufGetData(this->buffer, this->isize),
								this->isize);
					}
					TR_cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont = 0;
					break;
				}
				
				httpParserNewMessage(this, line, line_end);
				if (NULL == this->current) {
					TR_cbufRelease(this->buffer);
					this->ourLock = FALSE;
					return -2; // a server error occured can't process...
				}
				httpParserRequestVars(this);

				this->state = HTTP_MESSAGE_INTRO_DONE;

			case HTTP_MESSAGE_INTRO_DONE:
				if (NULL == (line = TR_cbufGetLine(this->buffer, &line_end))) {
					if (! TR_cbufIsEmpty(this->buffer)) {
						this->isize      = this->buffer->bused;
						this->incomplete = TR_malloc(this->isize);
						memcpy(this->incomplete,
								TR_cbufGetData(this->buffer, this->isize),
								this->isize);
					}
					TR_cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont = 0;
					break;
				}

				if (0 != strlen(line)) {
					httpParserHeader(this, line, line_end);
					break;
				}

				this->state = HTTP_MESSAGE_HEADERS_DONE;

			case HTTP_MESSAGE_HEADERS_DONE:
				if (this->current->dbody == this->current->nbody) {
					this->state = HTTP_MESSAGE_DONE;
				} else {
					if (TR_cbufIsEmpty(this->buffer)) {
						TR_cbufRelease(this->buffer);
						this->ourLock = FALSE;
						cont = 0;
						break;
					}

					TR_cbufIncRead(
							this->buffer,
							httpParserBody(
								this,
								TR_cbufGetRead(this->buffer),
								this->buffer->bused));

					break;
				}

			case HTTP_MESSAGE_DONE:
				{
					HttpHeader enc = TR_hashGet(
							this->current->header,
							CSTRA("content-type"));

					/**
					 * do we have form data??
					 */
					if (NULL != enc && 0 == strncasecmp(
								"application/x-www-form-urlencoded",
								enc->value[0],
								MIN(sizeof("application/x-www-form-urlencoded")-1,
									enc->nvalue[0]))) {
						//!> then parse them...
						httpParserPostVars(this);
					}

					/**
					 * enqueue current request
					 */
					TR_queuePut(this->queue, this->current);
					this->current = NULL;

					/**
					 * prepare for next request
					 */
					this->state = HTTP_MESSAGE_GARBAGE;
				}
				break;

			default:
				break;
		}
	}

	return this->queue->nmsg;
}

// vim: set ts=4 sw=4:
