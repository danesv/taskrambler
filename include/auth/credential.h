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

#ifndef __AUTH_CREDENTIAL_H__
#define __AUTH_CREDENTIAL_H__

#include <sys/types.h>

#include "class.h"

#define CRED_PWD(c)		(((c)->cred).pwd)

typedef enum e_CredentialType {
	CRED_PASSWORD = 0
} CredentialType;


CLASS(Credential) {
	CredentialType type;

	union {

		struct {
			char * user;
			size_t nuser;
			char * pass;
			size_t npass;
		} pwd;

	} cred;
};

#endif // __AUTH_CREDENTIAL_H__

// vim: set ts=4 sw=4:
