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
#include <sys/types.h>

#include "trbase.h"
#include "auth.h"
#include "auth/ldap.h"
#include "auth/storage.h"

int
authCreate(Auth this, AuthModule module, ...)
{
	va_list params;

	if (NULL != this->auth[module]) {
		TR_delete(this->auth[module]);
	}

	va_start(params, module);

	switch (module) {
		case AUTH_LDAP:
			this->auth[module] = TR_newv(AuthLdap, &params);
			break;

		case AUTH_STORAGE:
			this->auth[module] = TR_newv(AuthStorage, &params);
			break;
	}

	va_end(params);

	if (NULL == this->auth[module]) {
		return FALSE;
	}

	return module;
}

// vim: set ts=4 sw=4:
