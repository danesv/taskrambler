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

#include <stdarg.h>

#include "logger.h"
#include "interface/class.h"
#include "interface/logger.h"

const
char * const
logger_level_str[] = {
	"DEBUG",
	"INFO",
	"NOTICE",
	"WARNING",
	"ERR",
	"CRIT",
	"ALERT",
	"EMERG"
};

static
int
loggerCtor(void * _this, va_list * params)
{
	Logger this = _this;
	this->min_level = va_arg(*params, int);

	return 0;
}

static void loggerDtor(void * _this) {}

INIT_IFACE(Class, loggerCtor, loggerDtor, NULL);
CREATE_CLASS(Logger, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
