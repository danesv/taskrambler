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
#include <json/json.h>

#include "cclass.h"
#include "class.h"

char _called;

INIT_CLASS(MOCK_CLASS);

__construct(MOCK_CLASS)
{
    _called = 1;
    this->value = va_arg(* params, int);
}

__jsonConst(MOCK_CLASS)
{
    _called = 1;
    assert(json_type_int == json_object_get_type(json));

    this->value = json_object_get_int(json);
}

__clear(MOCK_CLASS) {}

__destruct(MOCK_CLASS)
{
    _called = 1;
}

__toJson(MOCK_CLASS)
{
    *json   = json_object_new_int(this->value);
    _called = 1;
}

/**
 * ~~~ method implementations ~~~~~~~~
 */

int
mock_class_getValue(MOCK_CLASS this)
{
    return this->value;
}

void
mock_class_setValue(MOCK_CLASS this, int value)
{
    this->value = value;
}

/**
 * ~~~ helper for mock assertions ~~~~~~~~
 */
void *
getConstruct()
{
    return __construct;
}

void *
getJsonConst()
{
    return __jsonConst;
}

// vim: set et ts=4 sw=4:
