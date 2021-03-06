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

#ifndef __UTILS_MEMORY_H__
#define __UTILS_MEMORY_H__

#define CSTRA(val)	val, sizeof(val)-1 //!< Const STRing Argument

#define MEM_FREE(seg)	(memFree((void **)&(seg)))

#include <sys/types.h>

void   * memMalloc(size_t);
void   * memCalloc(size_t, size_t);
void     memFree(void **);
size_t   memGetSize(void *);
void     memCleanup();

#endif // __UTILS_MEMORY_H__

// vim: set ts=4 sw=4:
