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

#ifndef __AUTH_LDAP_H__
#define __AUTH_LDAP_H__

#include <ldap.h>
#include <sys/types.h>

#include "trbase.h"

TR_CLASS(AuthLdap) {
	LDAP *     ldap;
	char *     url;
	char *     base_dn;
	int        version;
	size_t     nbase_dn;
};
TR_INSTANCE_INIT(AuthLdap);

#endif // __AUTH_LDAP_H__

// vim: set ts=4 sw=4:
