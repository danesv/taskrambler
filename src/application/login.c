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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "class.h"
#include "auth.h"

#include "utils/memory.h"
#include "application/application.h"


int
applicationLogin(
		Application this,
		Credential  credential,
		Session     session)
{
	size_t i;

	for (i=0; i<this->nauth; i++) {
		if (authenticate(this->auth[i], credential)) {
			session->user = new(User, NULL);

			switch (credential->type) {
				case CRED_PASSWORD:
					session->user->email  = CRED_PWD(credential).user;
					session->user->nemail = &CRED_PWD(credential).nuser;

					if (NULL == userLoad(session->user, this->users)) {
						session->user->email = NULL;
						session->user->nemail = NULL;
					}

					break;

				default:
					break;
			}

			return 1;
		}
	}

	return 0;
}

// vim: set ts=4 sw=4:
