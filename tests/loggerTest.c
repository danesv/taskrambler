/**
 * \file
 * loggerTest.c: tests for my logger class
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
#include <stdlib.h>

#include "runtest.h"
#include "class.h"
#include "logger.h"
#include "mock/mock_logger.h"


const char testname[] = "loggerTest";
Logger logger = NULL;


static
int
__setUp()
{
    return TEST_OK;
}
int (* const setUp)() = __setUp;

static
int
__tearDown()
{
    if (NULL != logger) {
        ASSERT_OBJECT(logger);
        delete(logger);
    }

    return TEST_OK;
}
int (* const tearDown)() = __tearDown;

static
int
testLoggerLevel()
{
    logger = new(MockLogger, LOGGER_ERR);

    ASSERT_INSTANCE_OF(MockLogger, logger);
    ASSERT_EQUAL(LOGGER_ERR, logger->min_level);

    loggerLog(logger, LOGGER_WARNING, "foo %d %s", 123, "bar");

    ASSERT_STRING_EQUAL("", ((MockLogger)logger)->message);

    loggerLog(logger, LOGGER_ERR, "foo %d %s", 123, "bar");
    ASSERT_STRING_EQUAL("[ERR] foo 123 bar", ((MockLogger)logger)->message);

    mockLoggerCleanMsg((MockLogger)logger);
    loggerLog(logger, LOGGER_CRIT, "foo %d %s", 123, "bar");
    ASSERT_STRING_EQUAL("[CRIT] foo 123 bar", ((MockLogger)logger)->message);

    return TEST_OK;
}

static
int
testLoggerStderr()
{
    logger = new(LoggerStderr, LOGGER_ERR);

    if (NULL == freopen("/dev/null", "w", stderr)) {
        return TEST_ERROR;
    }
    loggerLog(logger, LOGGER_ERR, "foo %d %s", 123, "bar");

    /**
     * \todo think about a way to assert something here
     */

    return TEST_OK;
}

static
int
testLoggerSyslog()
{
    logger = new(LoggerSyslog, LOGGER_ERR);

    loggerLog(logger, LOGGER_ERR, "foo %d %s", 123, "bar");

    /**
     * \todo think about a way to assert something here
     */

    return TEST_OK;
}

const testfunc tests[] = {
    testLoggerLevel,
    testLoggerStderr,
    testLoggerSyslog
};
const size_t count = FUNCS_COUNT(tests);

// vim: set et ts=4 sw=4:
