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

#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>

#include "http/message.h"
#include "http/request.h"
#include "http/response.h"

#include "trbase.h"

#define ALPHAVAL(x)     (tolower((x)) - 'a' + 0xa)
#define DIGITVAL(x)     ((x) - '0')
#define ALNUMVAL(x)     (isdigit((x))?DIGITVAL((x)):ALPHAVAL((x)))


static const char *DAY_NAMES[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


/*
 * This one is not thread save.
 */
size_t
rfc1123Gmt(char * buffer, size_t _nbuf, const time_t * t)
{
	struct tm * tmp = gmtime(t);
	size_t      nbuf;

	nbuf = strftime(buffer, _nbuf, "---, %d --- %Y %T GMT", tmp);
	memcpy(buffer, DAY_NAMES[tmp->tm_wday], 3);
	memcpy(buffer+8, MONTH_NAMES[tmp->tm_mon], 3);

	return nbuf;
}

/*
 * This one is not thread save.
 */
size_t
rfc1123GmtNow(char * buffer, size_t _nbuf)
{
	time_t t = time(NULL);

	return rfc1123Gmt(buffer, _nbuf, &t);
}

/**
 * Decode an url encoded string. This expects a valid url
 * encoded string and it size as arguments, else the behaviour
 * of this function is undefined.
 * This function modifies the data in buffer. No copy is made.
 * The reason for this is only performance.
 */
size_t
urldecode(char * buffer, size_t nbuffer)
{
    char * buf_ptr = buffer;
    char * res_ptr = buffer;

    for(; 0 < nbuffer; nbuffer--, buf_ptr++, res_ptr++) {
        switch(*buf_ptr) {
            case '%':
                *res_ptr = (ALNUMVAL(buf_ptr[1]) << 4) | ALNUMVAL(buf_ptr[2]);
                buf_ptr += 2;
                nbuffer -= 2;
                break;

            case '+':
                *buf_ptr = ' ';
                /* intended drop through */

            default:
                *res_ptr = *buf_ptr;
                break;
        }
    }
    *res_ptr = 0;

    return res_ptr - buffer;
}

char
isHttpVersion(const char * str, size_t len)
{
	if (NULL == str)
		return FALSE;

	if (8 > len)
		return FALSE;

	if (0 != memcmp("HTTP/", str, sizeof("HTTP/")-1))
		return FALSE;

	return TRUE;
}

HttpMessage
httpGetMessage(
		const char * part1, size_t len1,
		const char * part2, size_t len2,
		const char * part3, size_t len3)
{
	if (isHttpVersion(part1, len1)) {
		return TR_new(HttpResponse,
				part1, len1,
				strtoul(part2, NULL, 10),
				part3, len3);
	}

	if (isHttpVersion(part3, len3)) {
		return TR_new(HttpRequest,
				part1, len1,
				part2, len2,
				part3, len3);
	}

	return NULL;
}

// vim: set ts=4 sw=4:
