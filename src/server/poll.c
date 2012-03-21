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

#include <poll.h>
#include <errno.h>

#include "server.h"
#include "interface/logger.h"

#include "utils/signalHandling.h"

#define POLLFD(ptr)	((struct pollfd *)(ptr))

int
serverPoll(Server this) {
	int events;

	/**
	 * put all closed fds to end of array in O(this->nfds)
	 */
	struct pollfd * fda = &(this->fds[2]);
	struct pollfd * fdb = &(this->fds[this->nfds-1]);

	while (fda <= fdb) {
		while (0 == fdb->fd && fda <= fdb) {
			fdb--;
			this->nfds--;
		}

		while (0 != fda->fd && fda <= fdb) fda++;

		if (fda < fdb) {
			memcpy(fda, fdb, sizeof(struct pollfd));
			fdb--;
			this->nfds--;
		}
	}

	/*
	 * wait for handles to become ready
	 */
	if (-1 == (events = poll(this->fds, this->nfds, -1))) {
		switch (errno) {
			default:
			case EBADF:
			case EINVAL:
			case ENOMEM:
				doShutdown = 1;
				// DROP THROUGH

			case EINTR:
				loggerLog(this->logger, LOGGER_CRIT,
						"poll systemcall failed: [%s] - service terminated",
						strerror(errno));
		}
	}

	return events;
}

// vim: set ts=4 sw=4:
