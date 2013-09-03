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

#include "server.h"
#include "logger.h"

#include "utils/signalHandling.h"

int     serverPoll(Server);
int     serverHandleAccept(Server, unsigned int);
ssize_t serverRead(Server, unsigned int);
ssize_t serverWrite(Server, unsigned int);

void
serverRun(Server this)
{
    loggerLog(this->logger, LOGGER_INFO, "service started");

    while (!doShutdown) //! until error or signal 
    {
		int          events = 0;
		unsigned int i;

		if (0 == events) {
			events = serverPoll(this);
		}

		/**
		 * handle accept
		 */
		if (0 != ((this->fds)[0].revents & POLLIN)) {
			if (-1 == serverHandleAccept(this, 0)) {
				(this->fds)[0].revents &= ~POLLIN;
				events--;
			}
		}

		/**
		 * handle accept SSL
		 */
		if (0 != ((this->fds)[1].revents & POLLIN)) {
			if (-1 == serverHandleAccept(this, 1)) {
				(this->fds)[1].revents &= ~POLLIN;
				events--;
			}
		}

		for (i=2; i < this->nfds; i++) {
			/**
			 * handle reads 
			 */
			if (0 != ((this->fds)[i].revents & POLLIN)) {
				ssize_t processed = serverRead(this, i);

				if (0 < processed) {
					(this->fds)[i].revents &= ~POLLIN;
					events--;
				}

				if (processed > 0) {
					(this->fds)[i].events |= POLLOUT;
				}
			}

			/**
			 * handle writes
			 */
			if (0 != ((this->fds)[i].revents & POLLOUT)) {
				ssize_t remaining = serverWrite(this, i);

				if (0 > remaining) {
					events--;

					switch (remaining) {
						case -1: // poll me again
							(this->fds)[i].revents &= ~POLLOUT;
							break;

						case -2: // close me...
							serverCloseConn(this, i);
							break;
					}
				}

				if (0 == remaining) {
					(this->fds)[i].events &= ~POLLOUT;
				}
			}

			if (0 > events)
				break; // no more events to handle
		}
    }
}

// vim: set ts=4 sw=4:
