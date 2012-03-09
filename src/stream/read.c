#include <openssl/ssl.h>
#include <unistd.h>

#include "stream.h"

ssize_t
streamRead(Stream this, void * buf, size_t count)
{
	ssize_t done;

	switch(this->type) {
		case STREAM_FD:
			done = read((this->handle).fd, buf, count);
			break;

		case STREAM_SSL:
			done = SSL_read((this->handle).ssl, buf, count);
			break;

		default:
			break;
	}

	return done;
}

// vim: set ts=4 sw=4:
