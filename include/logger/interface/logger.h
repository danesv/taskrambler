/**
 * \file
 * The logger interface.
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

#ifndef __LOGGER_INTERFACE_LOGGER_H__
#define __LOGGER_INTERFACE_LOGGER_H__

#include <stdarg.h>

#include "trbase.h"
#include "logger.h"

typedef void (* fptr_log)(void *, logger_level, const char * const);

TR_INTERFACE(Logger) {
	TR_IFID;
	fptr_log log;
};

extern void loggerLog(void *, logger_level, const char * const, ...);

#endif // __LOGGER_INTERFACE_LOGGER_H__

// vim: set ts=4 sw=4:
