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

#include "class.h"
#include "queue.h"

static
int
queueCtor(void * _this, va_list * params)
{
	return 0;
}

static
void
queueDtor(void * _this)
{
	Queue this = _this;
	Queue node = this->first;
	
	while (NULL != node) {
		Queue next = node->next;
		delete(node->msg);
		delete(node);
		node = next;
	}
}

INIT_IFACE(Class, queueCtor, queueDtor, NULL);
CREATE_CLASS(Queue, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
