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

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <unistd.h>
#include <errno.h>

#include "stream.h"
#include "logger.h"

extern Logger logger;


ssize_t
streamRead(Stream this, void * buf, size_t count)
{
	ssize_t done;

	switch(this->type) {
		ssize_t _read;

		case STREAM_FD:
			_read = read((this->handle).fd, buf, count);

			if (_read < 0) {
				switch (errno) {
					case EINTR:
					case ENOMEM:
						done = 0;
						break;
					case (EAGAIN|EWOULDBLOCK):
						done = -1;
						break;
					default:
						done = -2;
						break;
				}
			} else if (_read == 0) {
				done = -2;
			} else {
				done = _read;
			}

			break;

		case STREAM_SSL:
			done = SSL_read((this->handle).ssl, buf, count);

			if (0 == done) {
				done = -2;
			} else if (0 > done) {
				switch (SSL_get_error((this->handle).ssl, done)) {
                    case SSL_ERROR_SYSCALL:
						{
							switch (errno) {
								case EINTR:
								case ENOBUFS:
								case ENOMEM:
									done = 0;
									break;
								case (EAGAIN|EWOULDBLOCK):
									done = -1;
									break;
								default:
									done = -1;
									break;
							}
						}
						break;

					case SSL_ERROR_SSL:
						{
							unsigned long err;

							while (0 != (err = ERR_get_error())) {
								loggerLog(
										logger,
										LOGGER_DEBUG,
										ERR_error_string(err, NULL));
							}
						}
						// DROP THROUGH

					case SSL_ERROR_ZERO_RETURN:
						done = -2;
						break;
				}
			}

			break;

		default:
			done = -2;
			break;
	}

	return done;
}

// vim: set ts=4 sw=4:
