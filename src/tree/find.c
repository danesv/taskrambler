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

#include "tree.h"

void *
treeFind(Tree this, const void * search, TreeComp comp)
{
	while (NULL != this) {
		int comparison = comp(this->data, search);

		if (0 < comparison) {
			this = TREE_LEFT(this);
			continue;
		}

		if (0 > comparison) {
			this = TREE_RIGHT(this);
			continue;
		}

		if (0 == comparison) {
			break;
		}
	}   

	return NULL != this ? this->data : NULL;
}

// vim: set ts=4 sw=4:
