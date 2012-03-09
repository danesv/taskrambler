/**
 * \file
 * runtest.c: the main runner for my tests
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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "runtest.h"
#include "cclass.h"


#define TEST_OK_CHAR		'.'
#define TEST_FAILED_CHAR	'F'
#define TEST_ERROR_CHAR		'E'


const char results[3] = {
    TEST_OK_CHAR,
    TEST_FAILED_CHAR,
    TEST_ERROR_CHAR
};

int
isObjectNull(void * _object)
{
    const CCLASS * class = _object - sizeof(CCLASS);

    if (! isObject(_object)) {
        return 0;
    }

    return isMemNull(_object, (*class)->size);
}

int
isMemNull(void * _mem, size_t size)
{
    size_t index;

    if (NULL == _mem) {
        return 0;
    }

    for(index=0; index<size && 0 == ((char *)_mem)[index]; index++);

    return (size == index);
}

int
main(int argc, char * argv[])
{
    size_t errors     = 0;
    size_t failures   = 0;
    // size_t assertions = 0;   // @TODO find a way to count assertions

    size_t index;

    printf("running tests for %s\n", testname);

    for (index=0; index<count; index++) {
        int result = TEST_ERROR, _setUp = 0;  // initialize setup to false

        if (NULL != setUp) {
            if (TEST_OK == (result = setUp())) {
                _setUp = 1;  // we successfully set up the test
            }
        }

        if (_setUp) {
            result = tests[index]();
        }

        if (_setUp && NULL != tearDown) {
            int _tearDown = tearDown();

            if ((! TEST_OK == _tearDown) && TEST_OK == result) {
                result = _tearDown;
            }
        }

        switch (result) {
            case TEST_FAILED: failures++; break;
            case TEST_ERROR:  errors++; break;
        }

        putchar(results[result]);

        if (79 == index%80) {
            putchar('\n');
        }

        fflush(stdout);
    }
    puts("\n");

    printf("running %lu tests: %lu - OK, %lu - FAILED, %lu - ERRORS\n",
            count,
            count - errors - failures,
            failures,
            errors);

    return failures + errors;
}

// vim: set et ts=4 sw=4:
