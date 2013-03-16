/**
 * \file
 * Definition of the observer pattern implementation.
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

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

typedef void (* fptr_observerNotify)(void *, void*);

extern const struct interface i_Observer;

struct i_Observer {
	const struct interface * const _;
	fptr_observerNotify      notify;
};

extern void observerNotify(void *, void *);

#endif // __OBSERVER_H__

// vim: set ts=4 sw=4:
