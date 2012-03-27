#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "class.h"
#include "stream.h"
#include "mock_worker.h"

static
int
mockWorkerCtor(void * _this, va_list * params)
{
	MockWorker this = _this;

	this->rbuf = &(this->_rbuf[0]);
	this->wbuf = &(this->_wbuf[0]);

	return 0;
}

static
void
mockWorkerDtor(void * _this)
{
}

static
void
mockWorkerClone(void * _this, void * _base)
{
	MockWorker this = _this;
	MockWorker base = _base;

	this->rbuf = &(base->_rbuf[0]);
	this->wbuf = &(base->_wbuf[0]);
}

static
ssize_t
mockWorkerRead(void * _this, Stream st)
{
	MockWorker this = _this;
	size_t size;

	size = read((st->handle).fd, this->rbuf, 1024);

	return size;
}

static
ssize_t
mockWorkerWrite(void * _this, Stream st)
{
	return 0;
}

INIT_IFACE(Class, mockWorkerCtor, mockWorkerDtor, mockWorkerClone);
INIT_IFACE(StreamReader, mockWorkerRead);
INIT_IFACE(StreamWriter, mockWorkerWrite);
CREATE_CLASS(
		MockWorker,
		NULL,
		IFACE(Class),
		IFACE(StreamReader),
		IFACE(StreamWriter));

// vim: set ts=4 sw=4:
