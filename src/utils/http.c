#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "http/message.h"
#include "http/request.h"
#include "http/response.h"

#include "interface/class.h"

#include "commons.h"

char
isHttpVersion(const char * str, size_t len)
{
	if (NULL == str)
		return FALSE;

	if (8 > len)
		return FALSE;

	if (0 != memcmp("HTTP/", str, sizeof("HTTP/")-1))
		return FALSE;

	return TRUE;
}

HttpMessage
httpGetMessage(
		const char * part1, size_t len1,
		const char * part2, size_t len2,
		const char * part3, size_t len3)
{
	if (isHttpVersion(part1, len1)) {
		return new(HttpResponse,
				part1, len1,
				strtoul(part2, NULL, 10),
				part3, len3);
	}

	if (isHttpVersion(part3, len3)) {
		return new(HttpRequest,
				part1, len1,
				part2, len2,
				part3, len3);
	}

	return NULL;
}

// vim: set ts=4 sw=4:
