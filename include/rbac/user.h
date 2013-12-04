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

#ifndef __RBAC_USER_H__
#define __RBAC_USER_H__

#include <sys/types.h>

#include "trbase.h"


/**
 * this has to implement serializeable.
 * Well maybe it hasn't...to be honest I think this
 * is more part of my storage class...if I would use
 * a relational database the handling would be completely
 * different when it comes to storage...it's somewhat
 * tricky to find the correct interface when we want to 
 * be flexible in future.
 */
TR_CLASS(RbacUser) {
	char   * name;
	size_t   nname;
};

// void userSerialize(User, void **, size_t *);
// void userDeserialize(User, void *, size_t);

#endif // __USER_H__

// vim: set ts=4 sw=4:

