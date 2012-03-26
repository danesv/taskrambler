/**
 * \file
 * mock/class.c: a mock to test my oop stuff
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

#include <assert.h>
#include <stdarg.h>

#include "class.h"
#include "mock_class.h"

char _called;

void
_reset()
{
    _called = 0;
}

static
inline
int
mockCtor(void * _this, va_list * params)
{
    MockClass this = _this;

    _called = 1;
    this->value = va_arg(* params, int);

    return 0;
}

static
inline
void
mockDtor(void * _this)
{
    _called = 1;
}

INIT_IFACE(Class, mockCtor, mockDtor, NULL);
CREATE_CLASS(MockClass, NULL, IFACE(Class));

/**
 * ~~~ method implementations ~~~~~~~~
 */

int
mockClassGetValue(MockClass this)
{
    return this->value;
}

void
mockClassSetValue(MockClass this, int value)
{
    this->value = value;
}

/**
 * ~~~ helper for mock assertions ~~~~~~~~
 */
void *
getConstruct()
{
    return mockCtor;
}

// vim: set et ts=4 sw=4:
