/**
 * \file
 * cclassTest.c: tests for my oop C stuff
 * Copyright (C) 2011  Georg Hopp
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
#include <sys/types.h>

#include "runtest.h"
#include "mock/mock_class.h"

#include "class.h"

const char testname[] = "cclassTest";

MockClass mock = NULL;

static
int
__setUp()
{
    mock = NULL;
    _reset();

    return TEST_OK;
}
int (* const setUp)() = __setUp;

static
int
__tearDown()
{
    if (NULL != mock) {
        ASSERT_OBJECT(mock);
        delete(mock);
    }

    return TEST_OK;
}
int (* const tearDown)() = __tearDown;

static
int
testNew(void)
{
    mock = new(MockClass, 123);

    ASSERT_OBJECT_NOT_NULL(mock);
    ASSERT_EQUAL(1, _called);
    ASSERT_EQUAL(123, mockClassGetValue(mock));

    return TEST_OK;
}

static
int
testDelete(void)
{
    mock = new(MockClass, 123);

    ASSERT_NOT_NULL(mock);

    _reset();
    delete(mock);

    ASSERT_NULL(mock);
    ASSERT_EQUAL(1, _called);

    return TEST_OK;
}

const testfunc tests[] = {
    testNew,
    testDelete,
};
const size_t count = FUNCS_COUNT(tests);

// vim: set et ts=4 sw=4:
