/**
 * \file
 * Container for authentication modules.
 *
 * This is a single point of authentication no matter how much
 * authentication modules are in place. Thus it prevents adding
 * more and more authentication modules to the application.
 * This is an auth module itself but this one returns 0 if
 * the authentication has failed otherwise the id of the
 * successfull auth module. Thus we can identify by what method
 * the user has been authenticated.
 *
 * This can't authenticate by its own. It has to be initialized
 * with other auth modules by calling authCreate at least once.
 *
 * origin intend ... never implemented (but maybe a good idea)
 *
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

#include <trbase.h>

#include "auth.h"
#include "auth/credential.h"


typedef enum e_AuthModule {
	AUTH_LDAP    = 1,
	AUTH_STORAGE = 2
} AuthModule;

#define MAX_AUTH	AUTH_STORAGE

TR_CLASS(Auth) {
	void * auth[MAX_AUTH + 1];
};
TR_INSTANCE_INIT(Auth);

int authCreate(Auth, AuthModule, ...);

#endif // __AUTH_AUTH_H__

// vim: set ts=4 sw=4:
