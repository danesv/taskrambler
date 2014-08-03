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

#include <trbase.h>
#include <trhash.h>
#include <trdata.h>

#include "user.h"


int
_controllerUpdateUserFromArgs(TR_Hash args, User * user)
{
	TR_HashValue email     = TR_hashGet(args, CSTRA("email"));
	TR_HashValue firstname = TR_hashGet(args, CSTRA("firstname"));
	TR_HashValue surname   = TR_hashGet(args, CSTRA("surname"));
	User      new_user;

	if (    
			NULL == email || 
			NULL == firstname ||
			NULL == surname)
	{   
		return FALSE;
	}

	new_user = TR_new(User,
			TR_uuidParse("14de9e60-d497-4754-be72-f3bed52541fc"),
			(char *)((*user)->username), *(*user)->nusername,
			(char *)(email->value), email->nvalue,
			(char *)(firstname->value), firstname->nvalue,
			(char *)(surname->value), surname->nvalue);

	TR_delete(*user);
	*user = new_user;

	return TRUE;
}

// vim: set ts=4 sw=4:
