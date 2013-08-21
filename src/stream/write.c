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

#include <openssl/ssl.h>
#include <unistd.h>

#include "stream.h"

ssize_t
streamWrite(Stream this, void * buf, size_t count)
{
	ssize_t done;

	switch(this->type) {
		case STREAM_FD:
			done = write((this->handle).fd, buf, count);
			break;

		case STREAM_SSL:
			done = SSL_write((this->handle).ssl, buf, count);
			break;

		default:
			done = 0;
			break;
	}

	return done;
}

// vim: set ts=4 sw=4:
