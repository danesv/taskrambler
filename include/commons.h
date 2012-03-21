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

#ifndef __COMMONS_H__
#define __COMMONS_H__

#define Bool	char
#define TRUE	1
#define FALSE	0

#ifndef MAX
#	define MAX(a,b)	((a)>(b)? (a) : (b))
#endif

#ifndef MIN
#	define MIN(a,b)	((a)<(b)? (a) : (b))
#endif 

#define SWAP_FUN(a, b)	((a)^=(b),(b)^=(a),(a)^=(b))

#define SWAP(type, a, b) do { \
	type tmp = (a);           \
	(a) = (b);                \
	(b) = tmp;                \
} while(0);

#endif // __COMMONS_H__

// vim: set ts=4 sw=4:
