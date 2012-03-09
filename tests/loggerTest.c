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
#include "cclass.h"
#include "logger.h"


int    level  = -1;
char * msg    = NULL;

static void
logfnct_mock(int _level, const char * _msg)
{
    level = _level;
    msg   = malloc(strlen(_msg) + 1);
    strcpy(msg, _msg);
}

const char testname[] = "loggerTest";
LOGGER logger = NULL;


static
int
__setUp()
{
    logger = new(LOGGER, NULL);

    ASSERT_INSTANCE_OF(LOGGER, logger);

    logger_add(logger, logfnct_mock);

    return TEST_OK;
}
int (* const setUp)() = __setUp;

static
int
__tearDown()
{
    level = -1;

    if (NULL != msg) {
        free(msg);
        msg = NULL;
    }

    if (NULL != logger) {
        ASSERT_OBJECT(logger);
        delete(&logger);
    }

    return TEST_OK;
}
int (* const tearDown)() = __tearDown;

static
int
testLogger()
{
    logger_log(logger, LOGGER_ERR, "foo %d %s", 123, "bar");

    ASSERT_EQUAL(LOGGER_ERR, level);
    ASSERT_STRING_EQUAL("foo 123 bar", msg);

    return TEST_OK;
}

const testfunc tests[] = {
    testLogger
};
const size_t count = FUNCS_COUNT(tests);

// vim: set et ts=4 sw=4:
