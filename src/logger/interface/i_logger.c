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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "logger/logger.h"
#include "logger/interface/logger.h"

#include "utils/memory.h"

const struct interface i_Logger = {
	"logger",
	1
};

void
loggerLog(void * _object, logger_level level, const char * const fmt, ...) {
	Logger object = _object;

	if (level >= object->min_level) {
		char    * msg      = NULL;
		size_t    msg_size = 0;
		va_list   params;

		va_start(params, fmt);
		msg_size = vsnprintf(NULL, msg_size, fmt, params);
		va_end(params);

		msg = memMalloc(msg_size + 1);

		va_start(params, fmt);
		vsnprintf(msg, msg_size + 1, fmt, params);
		va_end(params);

//		// ----- DEBUG ------
//		do {
//			struct i_Logger * iface;
//
//			do {
//				class_ptr class = GET_CLASS(_object);
//				iface = (struct i_Logger *)IFACE_GET(class, &i_Logger);
//				while ((NULL == iface || NULL == iface->log) && HAS_PARENT(class)) {
//					class = class->parent;
//					iface = (struct i_Logger *)IFACE_GET(class, &i_Logger);
//				}
//				assert(NULL != iface->log);
//			} while(0);
//
//			iface->log(_object, level, msg);
//		} while(0);
//		// ----- DEBUG ------
		CALL(_object, Logger, log, level, msg);

		MEM_FREE(msg);
	}
}

// vim: set ts=4 sw=4:
