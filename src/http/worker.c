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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <search.h>

#include "class.h"
#include "stream.h"
#include "hash.h"
#include "http/worker.h"
#include "http/parser.h"
#include "http/writer.h"

#include "utils/memory.h"

static
int
httpWorkerCtor(void * _this, va_list * params)
{
	HttpWorker this = _this;
	char *     id   = va_arg(*params, char *);
	char       cbuf_id[100];

	this->id  = memMalloc(strlen(id) + 1);
	strcpy(this->id, id);

	this->val = va_arg(*params, struct randval *);

	this->asset_pool = new(Hash);

	sprintf(cbuf_id, "%s_%s", "parser", id);
	this->pbuf   = new(Cbuf, cbuf_id, PARSER_MAX_BUF);

	sprintf(cbuf_id, "%s_%s", "writer", id);
	this->wbuf   = new(Cbuf, cbuf_id, WRITER_MAX_BUF);

	this->parser = new(HttpParser, this->pbuf);
	this->writer = new(HttpWriter, this->wbuf);

	this->sroot  = &(this->session);
	this->auth   = va_arg(* params, void *);

	return 0;
}

static
inline
void
tDelete(void * node)
{
	delete(node);
}

static
void
httpWorkerDtor(void * _this)
{
	HttpWorker this = _this;

	MEM_FREE(this->id);

	delete(this->parser);
	delete(this->writer);

	if (NULL != this->pbuf) {
		delete(this->asset_pool);
		delete(this->pbuf); //!< cloned workers have NULL, so delete won't do anything
		delete(this->wbuf); //!< cloned workers have NULL, so delete won't do anything
		tdestroy(*(this->sroot), tDelete);
	}
}

static
void
httpWorkerClone(void * _this, void * _base)
{
	HttpWorker this = _this;
	HttpWorker base = _base;

	this->val  = base->val;

	this->asset_pool = base->asset_pool;

	this->parser = new(HttpParser, base->pbuf);
	this->writer = new(HttpWriter, base->wbuf);

	this->sroot  = &(base->session);
	this->auth   = base->auth;
}

ssize_t httpWorkerProcess(void *, Stream);
ssize_t httpWorkerWrite(void *, Stream);

INIT_IFACE(Class, httpWorkerCtor, httpWorkerDtor, httpWorkerClone);
INIT_IFACE(StreamReader, httpWorkerProcess);
INIT_IFACE(StreamWriter, httpWorkerWrite);
CREATE_CLASS(
		HttpWorker,
		NULL, 
		IFACE(Class),
		IFACE(StreamReader),
		IFACE(StreamWriter));

// vim: set ts=4 sw=4:
