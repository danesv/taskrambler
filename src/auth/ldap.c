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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ldap.h>

#include "trbase.h"
#include "uuid.h"

#include "auth/ldap.h"
#include "auth/credential.h"
#include "auth/interface/auth.h"

static
int
authLdapCtor(void * _this, va_list * params)
{
	AuthLdap this = _this;
	char *   url  = va_arg(*params, char*);
	char *   base_dn;

	this->url = TR_malloc(strlen(url) + 1);
	strcpy(this->url, url);

	this->version  = 3;

	base_dn        = va_arg(* params, char *);
	this->nbase_dn = va_arg(* params, size_t);
	
	this->base_dn = TR_malloc(this->nbase_dn + 1);
	this->base_dn[this->nbase_dn] = 0;
	memcpy(this->base_dn, base_dn, this->nbase_dn);

	return 0;
}

static
void
authLdapDtor(void * _this)
{
	AuthLdap this = _this;

	TR_MEM_FREE(this->base_dn);
	TR_MEM_FREE(this->url);
}

static
int
authLdapAuthenticate(void * _this, Credential cred, Uuid user_index)
{
	AuthLdap this = _this;
	char     who[256];
	char *   who_ptr = who;
	int      ldap_err;

	struct berval   ldap_cred;
	struct berval * ldap_servcred;

	if (CRED_PASSWORD != cred->type) {
		return FALSE;
	}

	ldap_initialize(&(this->ldap), this->url);
	ldap_set_option(this->ldap, LDAP_OPT_PROTOCOL_VERSION, &(this->version));

	memcpy(who_ptr, "cn=", sizeof("cn=") - 1);
	who_ptr   += sizeof("cn=") - 1;
	memcpy(who_ptr, CRED_PWD(cred).user, CRED_PWD(cred).nuser);
	who_ptr += CRED_PWD(cred).nuser;
	*who_ptr++ = ',';
	memcpy(who_ptr, this->base_dn, this->nbase_dn);
	who_ptr[this->nbase_dn] = 0;

	ldap_cred.bv_val = CRED_PWD(cred).pass;
	ldap_cred.bv_len = CRED_PWD(cred).npass;
	ldap_err = ldap_sasl_bind_s(
			this->ldap,
			who,
			LDAP_SASL_SIMPLE,
			&ldap_cred,
			NULL,
			NULL,
			&ldap_servcred);

	ldap_unbind_ext_s(this->ldap, NULL, NULL);

	if (0 == ldap_err) {
		//! \todo here we need to get and return the user id
		return TRUE;
	}

	//fprintf(stderr, "%s\n", ldap_err2string(ldap_err));
	/** \todo do error logging instead. */
	return FALSE;
}

TR_INIT_IFACE(TR_Class, authLdapCtor, authLdapDtor, NULL);
TR_INIT_IFACE(Auth, authLdapAuthenticate);
TR_CREATE_CLASS(AuthLdap, NULL, TR_IF(TR_Class), TR_IF(Auth));

// vim: set ts=4 sw=4:
