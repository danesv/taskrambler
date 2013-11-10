/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2013  Georg Hopp
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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ldap.h>

#include "class.h"
#include "uuid.h"
#include "utils/memory.h"
#include "commons.h"

#include "auth.h"
#include "auth/credential.h"
#include "auth/interface/auth.h"

static
int
authCtor(void * _this, va_list * params)
{
	Auth this = _this;
	int  i;

	for (i=0; i<=MAX_AUTH; i++) {
		this->auth[i] = NULL;
	}

	return 0;
}

static
void
authDtor(void * _this)
{
	Auth this = _this;
	int  i;

	for (i=1; i<=MAX_AUTH; i++) {
		delete(this->auth[i]);
	}
}

static
int
authAuthenticate(void * _this, Credential cred, Uuid user_index)
{
	Auth this = _this;
	int  i;

	for (i=1; i<=MAX_AUTH; i++) {
		if (authenticate(this->auth[i], cred, user_index)) {
			return i;
		}
	}

	return FALSE;
}

INIT_IFACE(Class, authCtor, authDtor, NULL);
INIT_IFACE(Auth, authAuthenticate);
CREATE_CLASS(Auth, NULL, IFACE(Class), IFACE(Auth));

// vim: set ts=4 sw=4:
