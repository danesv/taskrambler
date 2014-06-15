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

#include "trbase.h"
#include "trdata.h"

#include "router.h"
#include "session.h"
#include "http/request.h"
#include "http/response.h"
#include "application/application.h"


#define COMMAND_LEN		128


HttpResponse
routerRoute(
		Router      this,
		HttpRequest request,
		Session     sess)
{
	char            functionName[COMMAND_LEN + this->nprefix * 10];
	TR_Hash         args = NULL;
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
			args = request->get;
			strcpy(&(functionName[this->nprefix + ncommand]), "Read");
			break;

		case HTTP_POST:
			args = request->post;
			strcpy(&(functionName[this->nprefix + ncommand]), "Create");
			break;

		case HTTP_PUT:
			args = request->post;
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
	 * Default policy should be deny, anyway, there are a few resource
	 * that should be accessible even when not logged in...the are at
	 * least most of the assets as well as functions like version or
	 * sessinfo and in fact currentuse to have a way to find out that
	 * one is not logged in.
	 * In general a deny will be handled by storing an error message in
	 * some stash and then trigger a redirect to the login page.
	 * To be really rbac it seems neccessary to me to create a user
	 * "not logged in" and assign him the exceptions to the default
	 * deny policy.
	 * For the moment I assume that if there is no resource for the
	 * URL in the application it must be an asset and just return NULL
	 * indication that we still have no response for the request.
	 * Another thought... resources will be created dynamically by
	 * creating tasks or users or anything.
	 * Each of these resources may have options to admin them. This means
	 * most of the time to be able to modify them but additionally the
	 * creater of the resource might need the right to modify the
	 * rbac rules that apply to that resource.
	 * So, if I keep the real resources and their rbac configuration
	 * separated as planned it might be neccessary to give the creater
	 * of a resource the ability to modify both.
	 *
	 * So lets assume user georg creates a task that might be identified
	 * by /task/uuid(task). Then additionally an rbac resource will be 
	 * created identified by /rbac_resource/uuid(/task/uuid(task)).
	 * User georg will have all rights on both resources.
	 * This means that rbac resources are resources by their own but how
	 * to control the access to these, I can't build another rbac resource
	 * and another and and and... so I think it is neccessary that every
	 * resource as it is has to hold their access in itself.
	 * The creating user will gain access to all REST operations as well
	 * as the right to change access control (which again are REST operations
	 * on these.
	 *
	 * Sidenote: I use a slightly differen naming than the ansi spec uses
	 *           I the term resource for object and action for operation.
	 *
	 * So most resources end up with the following set of possible actions:
	 *  - create: (well obviously this is only useful for list resources
	 *             eg. the tasklist of a new project)
	 *  - read:   be able to display the resource...
	 *            (again there is a special thing with lists here. This
	 *             only gives the right to see the list at all. When
	 *             generating the list the access rights on each entry
	 *             has to be checked and if there is no read right for it
	 *             it should not be included in the list.)
	 *  - update: be able to update a resource.
	 *            (this makes no sense for list resources as the change when
	 *             their members change)
	 *  - delete: be able to remove a resource.
	 *            (on list resources this should only be allowed if the list
	 *             is empty, this is the only consistent behaviour I can think
	 *             of because you can't always assume that by removing a
	 *             list ii's associated members should also be removed)
	 *  - rbac_read:
	 *  - rbac_update:
	 *
	 * Well, rbac assignes only roles to resources... in that case, how can I
	 * achieve per user rights for specific resources... one way would be
	 * to give every user its own role, which makes the whole concept kind
	 * of useless.
	 *
	 * Then I could allow everyone to create new roles on demand. Then
	 * a user would create a role that allows others to view the resource
	 * and then add user to this role. This role creation could be done
	 * automatically and in the UI the user simply only adds the users
	 * that should have access to the specific action.
	 * On the other hand the user might associate an action on the resource
	 * to an existing role.
	 * thus giving, for example, all team members the right to use the
	 * according action. Again in the UI this would be a simple select
	 * from a list.
	 * Still it seems neccessary to have a suer_private role where only 
	 * this one user is in and that has full access to all resource actions
	 * of each resource the user is creating...and if there is such a thing
	 * no new roles will be created when allowing others to take actions
	 * on specific resources...simply add the action to the private role of
	 * the other user.
	 * This private roles can be almost automatic.
	 * (created when user is created, removed when he is removed, etc. etc)
	 * Regarding the session...I hink it ok to use our sessions to store
	 * The resulting access rights defined by the roles the user is in.
	 * On the other hand...if we store them stere no immediate feedback is
	 * possible when one of the roles have been changed....well, maybe
	 * there is...each existing session for users that are associated with
	 * the changed role have to be updated. That is in any case better
	 * than calculating all the access right on every reqeust.
	 * So, what we have in place right now are users and sessions. Both
	 * can be extended to the needs for rbac.
	 * What we still need is a definition of resources and actions that
	 * build up a permission and roles in it self that will associate user
	 * with permissions.
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
			TR_delete(args);
			break;

		case HTTP_POST:
		case HTTP_PUT:
		case HTTP_DELETE:
		default:
			/* other methods are not subject of REST */
			break;
	}

	if (NULL != response_data) {
		response = httpResponseJson(response_data, strlen(response_data));
		TR_MEM_FREE(response_data);
	} else {
		response = httpResponse404();
	}

	return response;
}

// vim: set ts=4 sw=4:
