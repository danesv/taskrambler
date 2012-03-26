/**
 * \file
 * runtest.h: assertions and other definitions for all my tests
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
#ifndef __RUNTEST_h__
#define __RUNTEST_h__

#include <sys/types.h>
#include <string.h>


enum RESULT_TYPES {
    TEST_OK=0,
    TEST_FAILED,
    TEST_ERROR
};

#define ASSERT_NULL(value) \
    if (NULL != (value)) { \
        printf("%s[%d]: Assertion failed that %s is NULL\n", \
                __FILE__, __LINE__, #value); \
        return TEST_FAILED; }

#define ASSERT_NOT_NULL(value) \
    if (NULL == (value)) { \
        printf("%s[%d]: Assertion failed that %s is NOT NULL\n", \
                __FILE__, __LINE__, #value); \
        return TEST_FAILED; }

#define ASSERT_EQUAL(val1,val2) \
    if ((val1) != (val2)) { \
        printf("%s[%d]: Assertion failed that %s EQUALS %s\n", \
                __FILE__, __LINE__, #val1, #val2); \
        return TEST_FAILED; }

#define ASSERT_NOT_EQUAL(val1,val2) \
    if ((val1) == (val2)) { \
        printf("%s[%d]: Assertion failed that %s NOT EQUALS %s\n", \
                __FILE__, __LINE__, #val1, #val2); \
        return TEST_FAILED; }

#define ASSERT_MEM_EQUAL(val1,val2,size) \
    if(0 != memcmp((val1), (val2), (size))) { \
        printf("%s[%d]: Assertion failed that memory at %s EQUALS %s for %lu bytes\n", \
                __FILE__, __LINE__, #val1, #val2, size); \
        return TEST_FAILED; }

#define ASSERT_MEM_NOT_EQUAL(val1,val2,size) \
    if(0 == memcmp((val1), (val2), (size))) { \
        printf("%s[%d]: Assertion failed that memory at %s NOT EQUALS %s for %lu bytes\n", \
                __FILE__, __LINE__, #val1, #val2, size); \
        return TEST_FAILED; }

#define ASSERT_MEM_NULL(val, size) \
    if (! isMemNull((val), (size))) { \
        printf("%s[%d]: Assertion failed that memory at %s is NULL for %lu bytes\n", \
                __FILE__, __LINE__, #val, size); \
        return TEST_FAILED; }

#define ASSERT_MEM_NOT_NULL(val, size) \
    if (isMemNull((val), (size))) { \
        printf("%s[%d]: Assertion failed that memory at %s is NOT NULL for %lu bytes\n", \
                __FILE__, __LINE__, #val, size); \
        return TEST_FAILED; }

#define ASSERT_STRING_EQUAL(val1, val2) \
    if(0 != strcmp((val1), (val2))) { \
        printf("%s[%d]: Assertion failed that string %s EQUALS %s\n", \
                __FILE__, __LINE__, val1, val2); \
        return TEST_FAILED; }

#define ASSERT_STRING_NOT_EQUAL(val1, val2) \
    if(0 == strcmp((val1), (val2))) { \
        printf("%s[%d]: Assertion failed that string %s NOT EQUALS %s\n", \
                __FILE__, __LINE__, val1, val2); \
        return TEST_FAILED; }

#define ASSERT_OBJECT(val) \
    if (! isObject((val))) { \
        printf("%s[%d]: Assertion failed that %s IS an object\n", \
                __FILE__, __LINE__, #val); \
        return TEST_FAILED; }

#define ASSERT_OBJECT_NULL(val) \
    if (! isObjectNull((val))) { \
        printf("%s[%d]: Assertion failed that %s IS an UNINITIALIZED object\n", \
                __FILE__, __LINE__, #val); \
        return TEST_FAILED; }

#define ASSERT_OBJECT_NOT_NULL(val) \
    if (isObjectNull((val))) { \
        printf("%s[%d]: Assertion failed that %s IS an INITIALIZED object\n", \
                __FILE__, __LINE__, #val); \
        return TEST_FAILED; }

#define ASSERT_INSTANCE_OF(class, val) \
    if (! instanceOf(class, val)) { \
        printf("%s[%d]: Assertion failed that %s is instance of %s\n", \
                __FILE__, __LINE__, #val, #class); \
        return TEST_FAILED; }


typedef int (* const testfunc)(void);
#define FUNCS_COUNT(array)  (sizeof((array)) / sizeof(testfunc))

extern const char testname[];
extern testfunc tests[];
extern const size_t count;

extern int (* const setUp)();
extern int (* const tearDown)();

int isMemNull(void * _mem, size_t size);
int isObjectNull(void * _object);
int isObject(void * _object);

#endif//__RUNTEST_h__
// vim: set et ts=4 sw=4:
