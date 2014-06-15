/**
 * \file
 * Parse requests from an input stream.
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

#ifndef __HTTP_PARSER_H__
#define __HTTP_PARSER_H__

#include "trbase.h"
#include "trio.h"
#include "trdata.h"
#include "http/message.h"


#define PARSER_MAX_BUF	131072


typedef enum e_HttpMessageState {
	HTTP_MESSAGE_GARBAGE=0,
	HTTP_MESSAGE_START,
	HTTP_MESSAGE_INTRO_DONE,
	HTTP_MESSAGE_HEADERS_DONE,
	HTTP_MESSAGE_DONE
} HttpMessageState;


TR_CLASS(HttpParser) {
	TR_Cbuf          buffer;
	Bool             ourLock;

	char *           incomplete;
	size_t           isize;

	TR_Queue         queue;
	HttpMessage      current;

	HttpMessageState state;
};

ssize_t httpParserParse(void *, TR_Stream);
void    httpParserRequestVars(HttpParser);
void    httpParserHeader(HttpParser, const char *, const char *);
void    httpParserNewMessage(HttpParser, const char *, const char * lend);
size_t  httpParserBody(HttpParser, const char *, size_t);
void    httpParserRequestVars(HttpParser);
void    httpParserPostVars(HttpParser);

#endif // __HTTP_PARSER_H__

// vim: set ts=4 sw=4:
