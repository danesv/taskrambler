/**
 * \file
 * Authenticatio module factory
 *
 * A factory to get a specific authentication module.
 * An authentication module is a class that implement the Auth interface.
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

#ifndef __AUTH_AUTH_H__
#define __AUTH_AUTH_H__

#include "class.h"
#include "auth/ldap.h"

typedef enum e_AuthModule {
	AUTH_LDAP = 0
} AuthModule;

CLASS(Auth) {
};

void *   authCreateById(Auth, int);
AuthLdap authCreateLdap(Auth);

#endif // __AUTH_AUTH_H__

// vim: set ts=4 sw=4:
