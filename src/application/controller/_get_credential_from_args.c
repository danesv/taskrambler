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

#include "hash.h"
#include "auth/credential.h"

#include "utils/memory.h"
#include "commons.h"

int _controllerValidatePasswordRepeat(char *, size_t, char *, size_t);

Credential
_controllerGetCredentialFromArgs(Hash args)
{
	HashValue email     = hashGet(args, CSTRA("email"));
	HashValue password  = hashGet(args, CSTRA("password"));
	HashValue pwrepeat  = hashGet(args, CSTRA("pwrepeat"));

	if (    
			NULL == email || 
			NULL == password ||
			NULL == pwrepeat)
	{   
		return FALSE;
	}

	if (! _controllerValidatePasswordRepeat(
				password->value,
				password->nvalue,
				pwrepeat->value,
				pwrepeat->nvalue))
	{
		return FALSE;
	}

	return new(Credential,
			CRED_PASSWORD,
			(char *)(email->value), email->nvalue,
			(char *)(password->value), password->nvalue);
}

// vim: set ts=4 sw=4:
