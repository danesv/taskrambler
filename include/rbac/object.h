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

#ifndef __RBAC_OBJECT_H__
#define __RBAC_OBJECT_H__

#include <sys/types.h>

#include "trbase.h"
#include "storage/storage.h"


TR_CLASS(RbacObject) {
	char   * name;
	size_t   nname;
};
TR_INSTANCE_INIT(RbacObject);
TR_CLASSVARS_DECL(RbacObject) {};

#endif // __RBAC_OBJECT_H__

// vim: set ts=4 sw=4:

