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
void    serverCloseConn(Server, unsigned int);


void
serverRun(Server this)
{
	int events = 0;

    loggerLog(this->logger, LOGGER_INFO, "service started");

    while (!doShutdown) //! until error or signal 
    {
		unsigned int i;

		if (0 <= events) {
			/*
			 * TODO check why sometimes events is less than 0
			 * There is still a misshandling here.
			 */
			events = serverPoll(this);
		}

		/**
		 * handle accept
		 */
		if (0 != ((this->fds)[0].revents & POLLIN)) {
			if (0 > serverHandleAccept(this, 0)) {
				events--;
			}
		}

		/**
		 * handle accept SSL
		 */
		if (0 != ((this->fds)[1].revents & POLLIN)) {
			if (-1 == serverHandleAccept(this, 1)) {
				events--;
			}
		}

		for (i=2; i < this->nfds; i++) {
			/**
			 * handle reads 
			 */
			if (0 != ((this->fds)[i].revents & POLLIN)) {
				ssize_t processed = serverRead(this, i);

				if (0 > processed) {
					events--;

					switch (processed) {
						case -2: // close me...
							serverCloseConn(this, i);

						case -1: // poll me again
							break;
					}
				}

				if (0 < processed) {
					(this->fds)[i].events |= POLLOUT;
				}
			}

			/**
			 * handle writes
			 */
			if (0 != ((this->fds)[i].revents & POLLOUT)) {
				ssize_t remaining = serverWrite(this, i);

				if (0 >= remaining) {
					/*
					 * 0 means queue was empty...try again next
					 * time...no need to poll again.
					 * Anyway, most likely we need to read again
					 * so lets finish this event for now.
					 */
					events--;

					switch (remaining) {
						case  0: // nothing more to write stop polling
							(this->fds)[i].events &= ~POLLOUT;
							break;

						case -2: // close me...
							serverCloseConn(this, i);

						case -1: // poll me again
							break;
					}
				}
			}

			if (0 >= events)
				break; // no more events to handle
		}
    }
}

// vim: set ts=4 sw=4:
