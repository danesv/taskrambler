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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include "class.h"
#include "auth.h"
#include "queue.h"
#include "session.h"
#include "stream.h"
#include "hash.h"

#include "http/worker.h"
#include "http/header.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"
#include "http/parser.h"
#include "config/config.h"
#include "config/value.h"

#include "interface/subject.h"

#include "utils/memory.h"
#include "utils/mime_type.h"
#include "commons.h"


HttpMessage httpWorkerGetAsset(HttpWorker, const char *);
void        httpWorkerAddCommonHeader(HttpWorker);
void		httpWorkerAddComputedHeader(HttpWorker);

extern Config config;

ssize_t
httpWorkerProcess(HttpWorker this, Stream st)
{
	ssize_t requests = httpParserParse(this->parser, st);

	if (0 > requests) {
		return requests;
	}

	if (0 < requests) {
		while (! queueEmpty(this->parser->queue)) {
			this->current_request  = queueGet(this->parser->queue);
			this->current_response = NULL;

			/*
			 * let our observers...application (or better their
			 * http adapter) try to create an answer.
			 */
			subjectNotify(this);

			if (NULL == this->current_response) {
				if (0 == strcmp("POST", this->current_request->method) ||
						0 == strcmp("PUT", this->current_request->method))
				{
					/*
					 * we can't do post requests on our own...
					 */
					this->current_response = (HttpMessage)httpResponse500();
				}

				if (0 == strcmp("GET", this->current_request->method)) {
					ConfigValue assets_dir =
						configGet(config, CSTRA("assets_dir"));

					char asset_path[2048];

					char html_asset[] = "/assets/html";
					char base_asset[] = "/assets";
					char main_asset[] = "/main.html";

					char * asset;
					char * mime_type;

					strcpy(asset_path, (assets_dir->value).string);

					if (0 == strcmp("/", this->current_request->path)) {
						asset = main_asset;
					} else {
						asset = this->current_request->path;
					}

					mime_type = strrchr(asset, '.');
					if (NULL != mime_type) {
						mime_type++;
						mime_type = getMimeType(mime_type, strlen(mime_type));
					}

					if (NULL != mime_type &&
							0 == memcmp(mime_type, CSTRA("text/html"))) {
						strcat(asset_path, html_asset);
					} else {
						strcat(asset_path, base_asset);
					}

					strcat(asset_path, asset);
					this->current_response =
						httpWorkerGetAsset(this, asset_path);
				}
			}

			if (NULL == this->current_response) {
				this->current_response = (HttpMessage)httpResponse404();
			}

			httpWorkerAddCommonHeader(this);
			httpWorkerAddComputedHeader(this);
			delete(this->current_request);
			queuePut(this->writer->queue, this->current_response);
			this->current_response = NULL;
		}
	}

	return this->writer->queue->nmsg;
}

// vim: set ts=4 sw=4:
