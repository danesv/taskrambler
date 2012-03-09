/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
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

#include <sys/types.h>
#include <stdlib.h>

#include "interface.h"
#include "commons.h"

static
inline
int
comp(const void * _a, const void * _b)
{
	const struct interface * a = **(const struct interface ***)_a;
	const struct interface * b = **(const struct interface ***)_b;
    return ((a)<(b))? -1 : ((a)>(b))? 1 : 0;
}

/**
 * this one is important in selector functions to get the correct interface
 * implementation of a class.
 */
struct interface *
interfaceGet(iface_impl_ptr iface_impl, const iface_ptr _iface)
{
	const iface_ptr * iface = &_iface;
	void *            dummy;

    if (! iface_impl->simpl) {
        qsort((void**)(iface_impl->impl), iface_impl->nimpl, sizeof(iface_ptr), comp);
        iface_impl->simpl=TRUE;
    }

    dummy = bsearch(
			&iface,
			iface_impl->impl,
			iface_impl->nimpl,
			sizeof(iface_ptr),
			comp);

	return dummy? *(struct interface **)dummy : dummy;
}

// vim: set ts=4 sw=4:
