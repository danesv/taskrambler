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

#include <syslog.h>

#include "trbase.h"
#include "logger/logger.h"
#include "logger/interface/logger.h"

static
const
int syslog_priority[] = {
    LOG_USER | LOG_DEBUG,
    LOG_USER | LOG_INFO,
    LOG_USER | LOG_NOTICE,
    LOG_USER | LOG_WARNING,
    LOG_USER | LOG_ERR,
    LOG_USER | LOG_CRIT,
    LOG_USER | LOG_ALERT,
    LOG_USER | LOG_EMERG
};

static
void
logSyslog(void * this, logger_level level, const char * const msg)
{
    syslog(syslog_priority[level], "[%s] %s", logger_level_str[level], msg);
}

TR_INIT_IFACE(Logger, logSyslog);
TR_CREATE_CLASS(LoggerSyslog, Logger, TR_IF(Logger));

// vim: set ts=4 sw=4:
