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

#include "uuid.h"
#include "auth/auth.h"
#include "auth/credential.h"
#include "auth/interface/auth.h"

const struct interface i_Auth = {
	"auth",
	1
};

int
authenticate(void * auth, Credential cred, Uuid user_index)
{
	int ret;

	RETCALL(auth, Auth, authenticate, ret, cred, user_index);

	return ret;
}

// vim: set ts=4 sw=4:
