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

#include "trbase.h"
#include "auth/credential.h"


int
_controllerValidatePassword(
		char   * password,
		size_t   npassword,
		char   * pwrepeat,
		size_t   npwrepeat, )
{
	if (
			password->nvalue != pwrepeat->nvalue ||
			0 != memcmp(password->value, pwrepeat->value, password->nvalue))
	{
		return FALSE;
	}

	return TRUE;
}

// vim: set ts=4 sw=4:
