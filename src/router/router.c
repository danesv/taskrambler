/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2013  Georg Hopp
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

// for dlopen, dlsym
#include <dlfcn.h>

#include "class.h"
#include "router.h"
#include "hash.h"
#include "application/application.h"

#define PREFIX		"controller"

static
int
routerCtor(void * _this, va_list * params)
{
	Router this = _this;

	this->application = va_arg(*params, Application);
	this->functions   = new(Hash);
	this->handle      = dlopen(NULL, RTLD_LAZY);
	this->prefix      = PREFIX;
	this->nprefix     = sizeof(PREFIX) - 1;

	if (NULL == this->handle) {
		return -1;
	}

	return 0;
}

static
void
routerDtor(void * _this) {
	Router this = _this;

	delete(this->functions);
	dlclose(this->handle);
}

INIT_IFACE(Class, routerCtor, routerDtor, NULL);
CREATE_CLASS(Router, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
