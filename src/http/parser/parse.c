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

#include "http/parser.h"
#include "http/header.h"
#include "class.h"
#include "interface/http_intro.h"
#include "cbuf.h"
#include "stream.h"

#include "utils/memory.h"
#include "commons.h"

#define MIN(a,b)	((a)<(b)? (a) : (b))


ssize_t
httpParserParse(void * _this, Stream st)
{
	HttpParser this = _this;
	int        cont = 1;
	ssize_t    read;
	char *     line;
	char *     line_end;

	if (cbufIsLocked(this->buffer)) {
		if (FALSE == this->ourLock)
			return 0;
	}
	else {
		cbufLock(this->buffer);
		this->ourLock = TRUE;
	}

	if (NULL != this->incomplete) {
		cbufSetData(this->buffer, this->incomplete, this->isize);
		free(this->incomplete);
		this->incomplete = NULL;
	}

	if (0 > (read = cbufRead(this->buffer, st))) {
		return read;
	}

	while (cont) {
		switch(this->state) {
			case HTTP_MESSAGE_GARBAGE:
				cbufSkipNonAlpha(this->buffer);
				if (! cbufIsEmpty(this->buffer)) {
					this->state = HTTP_MESSAGE_START;
				}
				else {
					cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont          = 0;
				}

				break;

			case HTTP_MESSAGE_START:
				if (NULL == (line = cbufGetLine(this->buffer, &line_end))) {
					if (! cbufIsEmpty(this->buffer)) {
						this->isize = this->buffer->bused;
						this->incomplete = malloc(this->isize);
						memcpy(this->incomplete,
								cbufGetData(this->buffer, this->isize),
								this->isize);
					}
					cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont = 0;
					break;
				}
				
				httpParserNewMessage(this, line, line_end);
				if (NULL == this->current) {
					cbufRelease(this->buffer);
					this->ourLock = FALSE;
					return -1;
				}
				httpParserRequestVars(this);

				this->state = HTTP_MESSAGE_INTRO_DONE;
				break;

			case HTTP_MESSAGE_INTRO_DONE:
				if (NULL == (line = cbufGetLine(this->buffer, &line_end))) {
					if (! cbufIsEmpty(this->buffer)) {
						this->isize = this->buffer->bused;
						this->incomplete = malloc(this->isize);
						memcpy(this->incomplete,
								cbufGetData(this->buffer, this->isize),
								this->isize);
					}
					cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont = 0;
					break;
				}

				if (0 == strlen(line)) {
					this->state = HTTP_MESSAGE_HEADERS_DONE;
					break;
				}

				httpParserHeader(this, line, line_end);
				break;

			case HTTP_MESSAGE_HEADERS_DONE:
				if (this->current->dbody == this->current->nbody) {
					this->state = HTTP_MESSAGE_DONE;
					break;
				}

				if (cbufIsEmpty(this->buffer)) {
					cbufRelease(this->buffer);
					this->ourLock = FALSE;
					cont = 0;
					break;
				}

				cbufIncRead(
						this->buffer,
						httpParserBody(
							this,
							cbufGetRead(this->buffer),
							this->buffer->bused));
				break;

			case HTTP_MESSAGE_DONE:
				{
					HttpHeader enc = hashGet(
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
				this->queue->msgs[(this->queue->nmsgs)++] = this->current;
				this->current                             = NULL;

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

	return this->queue->nmsgs;
}

// vim: set ts=4 sw=4:
