/**
 * \file
 * mock/class.h: definitions for my mock to test my oop stuff
 *
 * \author Georg Hopp <georg@steffers.org>
 *
 * \copyright
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
#ifndef __MOCK_MOCK_CLASS_H__
#define __MOCK_MOCK_CLASS_H__

#include "class.h"

extern char _called;

#ifndef _RESET
#define _RESET
void _reset();
#endif // _RESET


CLASS(MockClass) {
    int value;
};

/**
 * ~~~ method declarations ~~~~~~~~
 */

int  mockClassGetValue(MockClass this);
void mockClassSetValue(MockClass this, int value);

#endif//__MOCK_MOCK_CLASS_H__

// vim: set et ts=4 sw=4:
