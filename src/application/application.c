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

#define _GNU_SOURCE

#include <stdarg.h>

#include "class.h"
#include "queue.h"
#include "application/application.h"
#include "storage/storage.h"

#include "utils/memory.h"

static
int
applicationCtor(void * _this, va_list * params)
{
	Application this = _this;
	size_t      i;

	this->val   = va_arg(*params, struct randval *);

	/*
	 * @TODO for both of these...each user should be identified
	 * by a number...that way I could use that number in the
	 * passwords db and no direct association between email and
	 * password could be made when someone get the hands on the
	 * password database.
	 */
	this->users     = va_arg(*params, Storage);
	this->passwords = va_arg(*params, Storage);

	// initialize authenticators to use.
	this->nauth = va_arg(*params, size_t);
	this->auth  = memMalloc(this->nauth * sizeof(void*));
	for (i=0; i<this->nauth; i++) {
		this->auth[i] = va_arg(*params, void *);
	}

	this->active_sessions = new(Queue);

	return 0;
}

static
void
applicationDtor(void * _this)
{
	Application this = _this;
	size_t      i;

	delete(this->active_sessions);

	for (i=0; i<this->nauth; i++) {
		delete(this->auth[i]);
	}

	MEM_FREE(this->auth);
}


INIT_IFACE(Class, applicationCtor, applicationDtor, NULL);
CREATE_CLASS(Application, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
