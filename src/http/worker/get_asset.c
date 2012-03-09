#include <sys/types.h>

#include "http/header.h"
#include "http/message.h"
#include "http/request.h"
#include "http/response.h"

#include "utils/memory.h"

HttpMessage
httpWorkerGetAsset(
		HttpRequest request,
		const char * fname,
		const char * mime,
		size_t       nmime)
{
	char *     match;
	size_t     nmatch;
	HttpHeader header;

	header = httpHeaderGet(
			&(((HttpMessage)request)->header),
			CSTRA("If-None-Match"));

	if (NULL == header) {
		match  = "";
		nmatch = 0;
	}
	else {
		match  = (header->value)[0];
		nmatch = (header->nvalue)[0];
	}

	return (HttpMessage)httpResponseAsset(
			fname, mime, nmime, match, nmatch);
}

// vim: set ts=4 sw=4:
