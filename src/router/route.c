/**
 * \file
 * This is the generic application router....
 * Here RBAC can take place as every resource is always requested
 * via an HTTP request.
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

// for strchr and others.
#include <string.h>

// for size_t
#include <sys/types.h>

// for dlopen, dlsym
#include <dlfcn.h>

// for toupper
#include <ctype.h>

#include "router.h"
#include "hash.h"
#include "session.h"
#include "http/request.h"
#include "http/response.h"
#include "application/application.h"

#include "utils/memory.h"
#include "commons.h"


#define COMMAND_LEN		128


HttpResponse
routerRoute(
		Router      this,
		HttpRequest request,
		Session     sess)
{
	char            functionName[COMMAND_LEN + this->nprefix * 10];
	Hash            args = NULL;
	fptr_routable   function;

	char          * tmp;
	char          * command;
	size_t          ncommand;
	char          * response_data;
	HttpResponse    response;

	if ('/' != request->uri[0]) {
		/*
		 * we only support absolute paths within our
		 * application
		 */
		return NULL;
	}

	command    = &(request->uri[1]);
	command[0] = toupper(command[0]);

	/*
	 * find end of command
	 */
	tmp = strchr(command, '/');
	if (NULL == tmp) {
		ncommand = strlen(command);
	} else {
		ncommand = tmp - command;
	}

	memcpy(functionName, this->prefix, this->nprefix);
	memcpy(&(functionName[this->nprefix]),
			command, MIN(COMMAND_LEN, ncommand));

	/**
	 * \todo
	 * now get all arguments if we have some
	 */
	
	/*
	 * following the crud pattern we map the first part
	 * of the uri and the request method to according
	 * function names.
	 */
	switch (request->method_id) {
		case HTTP_GET:
			args = new(Hash);
			strcpy(&(functionName[this->nprefix + ncommand]), "Read");
			break;

		case HTTP_POST:
			args = request->post;
			strcpy(&(functionName[this->nprefix + ncommand]), "Create");
			break;

		case HTTP_PUT:
			strcpy(&(functionName[this->nprefix + ncommand]), "Update");
			break;

		case HTTP_DELETE:
			strcpy(&(functionName[this->nprefix + ncommand]), "Delete");
			break;

		default:
			/* other methods are not subject of REST */
			return NULL;
	}

	/* 
	 * \todo for the moment I don't cache the found symbol...
	 * I don't even check if there was an error...the only thing
	 * I do is checking a NULL symbol and in that case don't
	 * handle the request here.
	 */
	dlerror();
	function = dlsym(this->handle, functionName);

	/**
	 * \todo somewhere here or above access control have to take place
	 */

	if (NULL == function) {
		/**
		 * nothing there to handle the request ... so leave it to the
		 * caller...
		 */
		char * error;

		if (NULL != (error = dlerror())) {
			/**
			 * \todo add logging...maybe.
			 */
		}

		return NULL;
	}

	/*
	 * function has to allocate the memory for reponse_date by using
	 * memMalloc.
	 */
	response_data = function(this->application, sess, args);

	switch (request->method_id) {
		case HTTP_GET:
			delete(args);
			break;

		case HTTP_POST:
		case HTTP_PUT:
		case HTTP_DELETE:
		default:
			/* other methods are not subject of REST */
			break;
	}

	response = httpResponseJson(response_data, strlen(response_data));
	MEM_FREE(response_data);

	return response;
}

// vim: set ts=4 sw=4:
