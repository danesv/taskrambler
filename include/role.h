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

#ifndef __ROLE_H__
#define __ROLE_H__

#include <sys/types.h>

#include "trbase.h"
#include "ebac/permission.h"
#include "storage/storage.h"


TR_CLASS(Role) {
	char            id[37];
	unsigned long   hash;

	char          * name;
	size_t          nname;

	/**
	 * \todo We need a good way to serialize a hash.
	 * If I can't find any I should choose a different
	 * data structure here...but I think there is a way.
	 * And as there is none I will change this to simple
	 * arrays and add an index to them that is a hash.
	 * The arrays could even be static.
	 * When a role is load the index on permissions and
	 * users is updated each time.
	 * The longer I think about it the less I think this
	 * role based approach is good...
	 * Each user is able to create resources (tasks,
	 * projects, etc.) but in the first place these should
	 * only be accessible for her/him. Then he should be
	 * able to allow other users or groups of users to
	 * access these but with each of these separately.
	 * So at least this can't be accomplished only with
	 * roles or else a role has to be created for every
	 * resource.
	 * OK, lets think about some roles:
	 * - owner (for each resorce ... this will enable us
	 *          to share ownership.)
	 * - viewer (for each resource ... these might only 
	 *           see the resource.)
	 * - projectmember (for each project resource)
	 * - teamlead (might have special rights for some projects
	 *             and be able to see what each projectmember
	 *             can see)
	 * - projectowner (well ... this might be owner of
	 *                 a project resource)
	 * - projectadmin (being able to change parts of the project
	 *                 but can't see everything...)
	 *
	 * I think I do not need an index here...because these are only
	 * used to setup a permission set within the user session...so
	 * these are simply added to this set.....
	 * With the users it is a different story...in fact i don't think
	 * that this relation should be stored here...this is an extra
	 * relation which is indexed by the usr name and credential and
	 * holds an array of roles he is in...on the other hand...if
	 * a role changes we need a fast way to get all users to update
	 * their permissions if they are online...
	 *
	 * New thoughts, a resource might be either a URL or a userid.
	 * When it is a userid the configured rights are valid for all
	 * resources owned by that user.
	 * Each grouped resource has less precedence than a specific
	 * one. That means, access rights can be removed for single
	 * resources after they have been added via a grouped resource.
	 * By default each user is assigned a role that allows him to
	 * do everything with every resource she/he has created.
	 * This role is also named after the user id.
	 */
	Permission * permissions;
	size_t       npermissions;

	User          * users;
	Hash            users_idx;
	// end og ROLE definition.
};
TR_INSTANCE_INIT(Role);

#endif // __ROLE_H__

// vim: set ts=4 sw=4:

