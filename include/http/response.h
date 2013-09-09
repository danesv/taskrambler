/**
 * \file
 * Represents one HTTP response.
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

#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <time.h>
#include <sys/types.h>

#include "class.h"
#include "http/message.h"
#include "session.h"
#include "asset.h"


CLASS(HttpResponse) {
	EXTENDS(HttpMessage);

	unsigned int status;
	char *       reason;
};

HttpResponse httpResponse304(
		const char *, size_t,
		const char *, size_t,
		const char *, size_t);
HttpResponse httpResponse403();
HttpResponse httpResponse404();
HttpResponse httpResponse500();
HttpResponse httpResponseMe();
HttpResponse httpResponseLoginForm();
HttpResponse httpResponseRandval(time_t, int);
HttpResponse httpResponseSession(Session);
HttpResponse httpResponseAsset(const char *, size_t);

#endif // __HTTP_RESPONSE_H__

// vim: set ts=4 sw=4:
