/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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

#include "http/parser.h"

#include "utils/http.h"

void
httpParserNewMessage(
		HttpParser   this,
		const char * line,
		const char * lend)
{
	const char * part1, * part2, * part3;
	size_t       len1, len2, len3;

	part1 = line;
	part2 = memchr(line, ' ', lend - line);

	if (NULL == part2) return;

	len1 = part2 - part1;
	for (; *part2 == ' ' && *part2 != 0; part2++);

	part3 = memchr(part2, ' ', lend - part2);

	if (NULL == part3) return;

	len2 = part3 - part2;
	for (; *part3 == ' ' && *part3 != 0; part3++);

	len3 = lend - part3;

	this->current = httpGetMessage(part1, len1, part2, len2, part3, len3);
}

// vim: set ts=4 sw=4:
