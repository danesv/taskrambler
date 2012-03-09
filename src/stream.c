#include <stdarg.h>
#include <openssl/ssl.h>

#include "class.h"
#include "interface/class.h"
#include "stream.h"


static
int
streamCtor(void * _this, va_list * params)
{
	Stream this = _this;
	this->type  = va_arg(* params, StreamHandleType);

	switch(this->type) {
		case STREAM_FD:
			(this->handle).fd = va_arg(* params, int);
			break;

		case STREAM_SSL:
			(this->handle).ssl = va_arg(* params, SSL*);
			break;

		default:
			return -1;
	}

	return 0;
}

static
void
streamDtor(void * _this)
{
}

INIT_IFACE(Class, streamCtor, streamDtor, NULL);
CREATE_CLASS(Stream, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
