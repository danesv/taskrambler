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

#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "credential.h"
#include "class.h"

#include "utils/memory.h"

static
int
credentialCtor(void * _this, va_list * params)
{
	Credential this = _this;
	
	this->type = va_arg(* params, CredentialType);

	switch(this->type) {
		case CRED_PASSWORD:
			{
				char * user, *pass;

				user                 = va_arg(* params, char*);
				CRED_PWD(this).nuser = va_arg(* params, size_t);
				pass                 = va_arg(* params, char*);
				CRED_PWD(this).npass = va_arg(* params, size_t);

				CRED_PWD(this).user  = malloc(CRED_PWD(this).nuser + 1);
				CRED_PWD(this).user[CRED_PWD(this).nuser] = 0;
				memcpy(CRED_PWD(this).user, user, CRED_PWD(this).nuser);

				CRED_PWD(this).pass  = malloc(CRED_PWD(this).npass + 1);
				CRED_PWD(this).pass[CRED_PWD(this).npass] = 0;
				memcpy(CRED_PWD(this).pass, pass, CRED_PWD(this).npass);
			}
			break;

		default:
			return -1;
	}

	return 0;
}

static
void
credentialDtor(void * _this)
{
	Credential this = _this;

	switch(this->type) {
		case CRED_PASSWORD:
			FREE(CRED_PWD(this).user);
			FREE(CRED_PWD(this).pass);
			break;
	}
}

INIT_IFACE(Class, credentialCtor, credentialDtor, NULL);
CREATE_CLASS(Credential, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
