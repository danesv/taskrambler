/**
 * \file
 * Holds requests ready for processing.
 * 
 * \todo change this to a real queue.
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

#ifndef __HTTP_MESSAGE_QUEUE_H__
#define __HTTP_MESSAGE_QUEUE_H__

#include <sys/types.h>

#include "class.h"
#include "http/message.h"
#include "commons.h"


CLASS(HttpMessageQueue) {
	HttpMessage      msg;
	HttpMessageQueue next;

	/**
	 * first and last are only available in the initial queue
	 * element (the root). This elelment does not contain any message
	 * and exists only for organizational purpose.
	 */
	HttpMessageQueue first;
	HttpMessageQueue last;
	size_t           nmsg;
};

void        httpMessageQueuePut(HttpMessageQueue, HttpMessage);
HttpMessage httpMessageQueueGet(HttpMessageQueue);

#define httpMessageQueueEmpty(this)		(0 >= (this)->nmsg)

#endif // __HTTP_MESSAGE_QUEUE_H__

// vim: set ts=4 sw=4:
