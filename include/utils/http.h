#ifndef __UTILS_HTTP_H__
#define __UTILS_HTTP_H__

#include <sys/types.h>

#include "http/message.h"

char        isHttpVersion(const char *, size_t);
HttpMessage httpGetMessage(
		const char *, size_t,
		const char *, size_t,
		const char *, size_t);

#endif // __UTILS_HTTP_H__

// vim: set ts=4 sw=4:
