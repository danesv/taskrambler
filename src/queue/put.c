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

#include "class.h"
#include "queue.h"

void
queuePut(Queue this, void * msg)
{
	Queue node = (this->last)? this->last : this;

	node->next = new(Queue);
	this->last = node->next;

	if (node == this) {
		this->first = node->next;
	}

	node->next->msg = msg;
	this->nmsg++;
}

// vim: set ts=4 sw=4:
