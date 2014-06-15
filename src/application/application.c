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

#include "trbase.h"
#include "trhash.h"
#include "trdata.h"

#include "application/application.h"
#include "storage/storage.h"

#include "config.h"

static
int
applicationCtor(void * _this, va_list * params)
{
	Application this = _this;
	size_t      i;

	this->val   = va_arg(*params, struct randval *);

	/**
	 * \todo for both of these...each user should be identified
	 * by a number...that way I could use that number in the
	 * passwords db and no direct association between email and
	 * password could be made when someone get the hands on the
	 * password database.
	 */
	this->users     = va_arg(*params, Storage);
	this->passwords = va_arg(*params, Storage);
	//this->roles     = va_arg(*params, Storage);

	this->user_namespace = va_arg(*params, TR_Uuid);

	this->auth = va_arg(*params, void *);

	this->active_sessions = TR_calloc(SESSION_LIVETIME, sizeof(TR_Hash));
	for (i=0; i<SESSION_LIVETIME; i++) {
		this->active_sessions[i] = TR_new(TR_Hash);
	}

	this->version = VERSION;
	this->loc     = LOC;

	return 0;
}

static
void
applicationDtor(void * _this)
{
	Application this = _this;
	size_t      i;

	TR_delete(this->user_namespace);

	for (i=0; i<SESSION_LIVETIME; i++) {
		TR_delete(this->active_sessions[i]);
	}

	TR_MEM_FREE(this->active_sessions);
	TR_MEM_FREE(this->auth);
}


TR_INIT_IFACE(TR_Class, applicationCtor, applicationDtor, NULL);
TR_CREATE_CLASS(Application, NULL, TR_IF(TR_Class));

// vim: set ts=4 sw=4:
