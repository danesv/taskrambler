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

void *
queueGet(Queue this)
{
	Queue  first;
	void * msg;

	if (NULL == this->first) {
		return NULL;
	}

	msg   = this->first->msg;
	first = this->first->next;

	if (this->first == this->last) {
		this->last = NULL;
	}
	delete(this->first);

	this->next  = first;
	this->first = first;
	this->nmsg--;

	return msg;
}

// vim: set ts=4 sw=4:
