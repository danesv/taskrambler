/**
 * \file
 * A generic REST router that is able to map a URL to a specific
 * function. It uses dlsym to get the address of the function to
 * be called.
 * The functions need to have a common interface for sure....This will
 * work out while I am working on this.
 * After a function is found it's address will be stored in a hash
 * so that further lookups might be faster.
 * By it's nature I think this is part of the HttpApplicationAdapter.
 *
 * \author  Georg Hopp
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

#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <dlfcn.h>

#include "trbase.h"
#include "trdata.h"

#include "session.h"
#include "http/request.h"
#include "http/response.h"
#include "application/application.h"

typedef char * (* fptr_routable)(Application, Session, TR_Hash);

TR_CLASS(Router) {
	TR_Hash     functions;
	Application application;

	void   * handle;
	char   * prefix;
	size_t   nprefix;
};

HttpResponse routerRoute(Router, HttpRequest, Session);

#endif // __ROUTER_H__

// vim: set ts=4 sw=4:
