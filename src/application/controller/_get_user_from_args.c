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

#define _GNU_SOURCE

#include <trbase.h>
#include <trhash.h>

#include "hash.h"
#include "user.h"


User
_controllerGetUserFromArgs(Hash args)
{
	HashValue email     = hashGet(args, CSTRA("email"));
	HashValue firstname = hashGet(args, CSTRA("firstname"));
	HashValue surname   = hashGet(args, CSTRA("surname"));

	if (    
			NULL == email || 
			NULL == firstname ||
			NULL == surname)
	{   
		return FALSE;
	}

	/**
	 * \todo how should we inject the user namespace here...
	 * or better, how to handle it at all...
	 * at least this is true for every controller...
	 * Additionally, this and the other controller functions
	 * that create a user in this way will leak memory.
	 * !!!IMPORTANT!!!
	 */
	return TR_new(User,
			TR_uuidParse("14de9e60-d497-4754-be72-f3bed52541fc"),
			(char *)(email->value), email->nvalue,
			(char *)(email->value), email->nvalue,
			(char *)(firstname->value), firstname->nvalue,
			(char *)(surname->value), surname->nvalue);
}

// vim: set ts=4 sw=4:
