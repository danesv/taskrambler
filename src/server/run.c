/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
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
#include "interface/logger.h"

#include "utils/signalHandling.h"

int     serverPoll(Server);
int     serverHandleAccept(Server, unsigned int);
void    serverCloseConn(Server, unsigned int);
ssize_t serverRead(Server, unsigned int);
ssize_t serverWrite(Server, unsigned int);

void
serverRun(Server this)
{
    loggerLog(this->logger, LOGGER_INFO, "service started");

    while (!doShutdown) //! until error or signal 
    {
		int          events;
		unsigned int i;
		int          naccs = 10;

		events = serverPoll(this);
		if (doShutdown || 0 >= events) break;

		/**
		 * handle accept
		 */
		if (0 != ((this->fds)[0].revents & POLLIN)) {
			events--;
			while(-1 != serverHandleAccept(this, 0) && 0 < naccs) {
				naccs--;
			}
		}

		/**
		 * handle accept SSL
		 */
		if (0 != ((this->fds)[1].revents & POLLIN)) {
			events--;
			while(-1 != serverHandleAccept(this, 1) && 0 < naccs) {
				naccs--;
			}
		}

		for (i=2; i < this->nfds; i++) {
			int nreads = 10, nwrites = 10;

			/**
			 * handle reads 
			 */
			if (0 != ((this->fds)[i].revents & POLLIN) && 0 < nreads) {
				events--;
				nreads--;

				serverRead(this, i);
			}

			/**
			 * handle writes
			 */
			if (0 != ((this->fds)[i].revents & POLLOUT) && 0 < nwrites) {
				events--;
				nwrites--;

				serverWrite(this, i);
			}

			if (0 > events)
				break; // no more events to handle
		}
    }
}

// vim: set ts=4 sw=4:
