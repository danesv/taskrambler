/**
 * \file
 * The authentication interface.
 *
 * This is the authentication interface. It's only pupose is to
 * authenticate someone or somewhat. It is called AUTH.
 * The concrete access rights are managed within a class called ACL.
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

#ifndef __INTERFACE_AUTH_H__
#define __INTERFACE_AUTH_H__

#include <stdarg.h>

#include "class.h"
#include "credential.h"

typedef int (* fptr_authenticate)(void *, Credential);

extern const struct interface i_Auth;

struct i_Auth {
	const struct interface * const _;
	fptr_authenticate              authenticate;
};

extern int authenticate(void *, Credential);

#endif // __INTERFACE_AUTH_H__

// vim: set ts=4 sw=4:
