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

#include "trbase.h"
#include "trio.h"
#include "http/worker.h"
#include "http/parser.h"
#include "http/writer.h"

static
int
httpWorkerCtor(void * _this, va_list * params)
{
	HttpWorker this = _this;
	char *     id   = va_arg(*params, char *);
	char       cbuf_id[100];

	this->id  = TR_malloc(strlen(id) + 1);
	strcpy(this->id, id);

	this->asset_pool = TR_new(TR_Hash);

	sprintf(cbuf_id, "%s_%s", "parser", id);
	this->pbuf   = TR_new(TR_Cbuf, cbuf_id, PARSER_MAX_BUF);

	this->additional_headers = TR_new(TR_Queue);

	this->parser = TR_new(HttpParser, this->pbuf);
	this->writer = TR_new(HttpWriter);

	return 0;
}

static
void
httpWorkerDtor(void * _this)
{
	HttpWorker this = _this;

	TR_MEM_FREE(this->id);

	TR_delete(this->additional_headers);

	TR_delete(this->parser);
	TR_delete(this->writer);

	if (NULL != this->pbuf) {
		TR_delete(this->asset_pool);
		TR_delete(this->pbuf); //!< cloned workers have NULL, so delete won't do anything
	}
}

static
void
httpWorkerClone(void * _this, void * _base)
{
	HttpWorker this = _this;
	HttpWorker base = _base;

	this->asset_pool          = base->asset_pool;
	this->application_adapter = base->application_adapter;

	this->additional_headers = TR_new(TR_Queue);

	this->parser = TR_new(HttpParser, base->pbuf);
	/*
	 * I am pretty sure that it is not neccessary to have a
	 * separeate writer for each connection...
	 * Right now I leave it that way.
	 * TODO check this.
	 * OK some facts:
	 * - the stream as well as the worker are associated
	 *   to the filehandle within the server.
	 * - the response queue is located within the writer.
	 *   (this might be wrong...the response queue should
	 *   be part of the worker. That way I could give it
	 *   into the writer when writing. That way only one
	 *   instance of the writer might be possible...)
	 *   NO, the previous statement is wrong...this would
	 *   involve much more organization overhead within
	 *   the writer...queue change and such...
	 *   At the end I think it might be best to leave it as
	 *   it is.
	 */
	this->writer = TR_new(HttpWriter);
}

ssize_t httpWorkerProcess(void *, TR_Stream);
ssize_t httpWorkerWrite(void *, TR_Stream);

static
void
httpWorkerDetach(void * _this, void * adapter)
{
	HttpWorker this = (HttpWorker)_this;

	if (NULL != this->application_adapter) {
		TR_delete(this->application_adapter);
	}
}

static
void
httpWorkerAttach(void * _this, void * adapter)
{
	HttpWorker this = (HttpWorker)_this;

	/*
	 * right now only one adapter is allowed and the last
	 * added will be used....all others will be deleted in
	 * assumption that no other handle does exist anymore
	 * (because it was added as an adapter and thus is good
	 * for nothing else.)
	 */
	httpWorkerDetach(_this, adapter);

	this->application_adapter = adapter;
}

static
void
httpWorkerNotify(void * _this)
{
	HttpWorker this = (HttpWorker)_this;

	TR_observerUpdate(this->application_adapter, _this);
}

TR_INIT_IFACE(TR_Class, httpWorkerCtor, httpWorkerDtor, httpWorkerClone);
TR_INIT_IFACE(TR_StreamReader, httpWorkerProcess);
TR_INIT_IFACE(TR_StreamWriter, httpWorkerWrite);
TR_INIT_IFACE(
		TR_Subject,
		httpWorkerAttach,
		httpWorkerDetach,
		httpWorkerNotify);
TR_CREATE_CLASS(
		HttpWorker,
		NULL, 
		TR_IF(TR_Class),
		TR_IF(TR_StreamReader),
		TR_IF(TR_StreamWriter),
		TR_IF(TR_Subject));

// vim: set ts=4 sw=4:
