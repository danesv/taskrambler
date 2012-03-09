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

#include <stdio.h>

#include "logger.h"
#include "interface/logger.h"

static
void
logStderr(void * this, logger_level level, const char * const msg)
{
	fprintf(stderr, "[%s] %s\n", logger_level_str[level], msg);
}

INIT_IFACE(Logger, logStderr);
CREATE_CLASS(LoggerStderr, Logger, IFACE(Logger));

// vim: set ts=4 sw=4:
