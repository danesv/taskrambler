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

#define _GNU_SOURCE

#include "trbase.h"
#include "trdata.h"
#include "user.h"
#include "auth/credential.h"


User       _controllerGetUserFromArgs(TR_Hash args);
Credential _controllerGetCredentialFromArgs(TR_Hash args);

int
_controllerProcessUserCreateArgs(TR_Hash args, User * user, Credential * cred)
{
	*user = _controllerGetUserFromArgs(args);
	*cred = _controllerGetCredentialFromArgs(args);
	
	if (NULL == *user || NULL == *cred) {   
		TR_delete(*user);
		TR_delete(*cred);

		return FALSE;
	}

	return TRUE;
}

// vim: set ts=4 sw=4:
