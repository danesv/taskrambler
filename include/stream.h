#ifndef __STREAM_H__
#define __STREAM_H__

#include <sys/types.h>
#include <openssl/ssl.h>

#include "class.h"

typedef enum e_StreamHandleType {
	STREAM_FD = 0,
	STREAM_SSL
} StreamHandleType;

CLASS(Stream) {
	StreamHandleType type;
	union {
		int   fd;
		SSL * ssl;
	}              handle;
};

ssize_t streamRead(Stream, void *, size_t);
ssize_t streamWrite(Stream, void *, size_t);

#endif // __STREAM_H__

// vim: set ts=4 sw=4:
