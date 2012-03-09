/**
 * \file
 * Holds requests ready for processing.
 * 
 * \todo change this to a real queue.
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
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

#include "class.h"
#include "http/message.h"

#define HTTP_MESSAGE_QUEUE_MAX	1024


CLASS(HttpMessageQueue) {
	HttpMessage msgs[HTTP_MESSAGE_QUEUE_MAX];
	size_t      nmsgs;
};

#endif // __HTTP_MESSAGE_QUEUE_H__

// vim: set ts=4 sw=4:
