/**
 * \file
 * A generic logger class and two extended classes, One that logs to
 * stderr and one that logs to the system syslog.
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

#ifndef __LOGGER_LOGGER_H__
#define __LOGGER_LOGGER_H__

#include "class.h"

typedef enum logger_level {
	LOGGER_DEBUG=0,
	LOGGER_INFO,
	LOGGER_NOTICE,
	LOGGER_WARNING,
	LOGGER_ERR,
	LOGGER_CRIT,
	LOGGER_ALERT,
	LOGGER_EMERG
} logger_level;

extern const char * const logger_level_str[];

CLASS(Logger) {
	logger_level min_level;
};

CLASS(LoggerStderr) {
	EXTENDS(Logger);
};

CLASS(LoggerSyslog) {
	EXTENDS(Logger);
};

#endif // __LOGGER_LOGGER_H__

// vim: set ts=4 sw=4:
