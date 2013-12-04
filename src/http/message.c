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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "trbase.h"
#include "hash.h"
#include "http/message.h"


static
int
httpMessageCtor(void * _this, va_list * params)
{
	HttpMessage this    = _this;
	char *      version = va_arg(* params, char *);

	this->version = TR_calloc(1, strlen(version)+1);
	strcpy(this->version, version);

	this->header = TR_new(Hash);

	return 0;
}

static
void
httpMessageDtor(void * _this)
{
	HttpMessage this = _this;

	TR_delete(this->header);
	TR_MEM_FREE(this->version);

	if (NULL == this->asset) {
		TR_MEM_FREE(this->body);
	} else {
		assetPoolRelease(this->asset);
		this->asset = NULL;
		this->body  = NULL;
	}
} 

TR_INIT_IFACE(TR_Class, httpMessageCtor, httpMessageDtor, NULL);
TR_CREATE_CLASS(HttpMessage, NULL, TR_IF(TR_Class));

// vim: set ts=4 sw=4:
