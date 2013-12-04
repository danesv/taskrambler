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

#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "trbase.h"
#include "cbuf.h"


static void cbufDtor(void*);

static
int
cbufCtor(void * _this, va_list * params)
{
	Cbuf   this     = _this;
	char   state    = -1;
	char * shm_name = va_arg(*params, char*);
	long   psize    = sysconf(_SC_PAGESIZE);
	size_t size;
	int    shm;
	char * data;

	this->shm_name = TR_malloc(strlen(shm_name) + 7 + 2);
	sprintf(this->shm_name, "/%06d_%s", getpid(), shm_name);

	/**
	 * align size at page boundary.
	 * increase as neccessary
	 */
	size = va_arg(*params, size_t);
	size = (0 >= size)? 1 : (0 != size%psize)? (size/psize)+1 : size/psize;
	this->bsize = psize * size;

	while (-1 == state) {
		shm = shm_open(this->shm_name, O_RDWR|O_CREAT|O_EXCL, S_IRWXU);
		if (-1 == shm) {
			break;
		}

		if (-1 == ftruncate(shm, this->bsize)) {
			break;
		}

		this->data = mmap (0, this->bsize << 1,
				PROT_NONE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
		if (this->data == MAP_FAILED) {
			this->data = NULL;
			break;
		}   

		data = mmap (this->data, this->bsize,
				PROT_READ|PROT_WRITE, MAP_FIXED|MAP_SHARED, shm, 0);
		if (data != this->data) {
			break;
		}   

		data = mmap (this->data + this->bsize, this->bsize,
				PROT_READ|PROT_WRITE, MAP_FIXED|MAP_SHARED, shm, 0);
		if (data != this->data + this->bsize) {
			break;
		}   

		state = 0;
	}

	if (-1 != shm) {
		shm_unlink(this->shm_name);
		close(shm);
	}

	return state;
}

static
void
cbufDtor(void * _this)
{
	Cbuf this = _this;

	TR_MEM_FREE(this->shm_name);

	if (NULL != this->data && MAP_FAILED != this->data) {
		munmap(this->data, this->bsize << 1);
	}

	this->data = NULL;
}

TR_INIT_IFACE(TR_Class, cbufCtor, cbufDtor, NULL);
TR_CREATE_CLASS(Cbuf, NULL, TR_IF(TR_Class));

// vim: set ts=4 sw=4:
