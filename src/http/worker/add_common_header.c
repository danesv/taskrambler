#include <time.h>
#include <sys/types.h>

#include "class.h"
#include "interface/class.h"

#include "http/message.h"
#include "http/response.h"

#include "utils/memory.h"

void
httpWorkerAddCommonHeader(HttpMessage request, HttpMessage response)
{
	time_t       t;
	struct tm *  tmp;
	char         buffer[200];
	size_t       nbuf;

	if (httpMessageHasKeepAlive(request)) {
		httpHeaderAdd(
				&(response->header),
				new(HttpHeader, CSTRA("Connection"), CSTRA("Keep-Alive")));
	}
	else {
		httpHeaderAdd(
				&(response->header),
				new(HttpHeader, CSTRA("Connection"), CSTRA("Close")));
	}

	httpHeaderAdd(&(response->header),
			new(HttpHeader, CSTRA("Server"), CSTRA("testserver")));

	switch(((HttpResponse)response)->status) {
		case 304:
			break;

		default:
			nbuf = sprintf(buffer, "%d", response->nbody);
			httpHeaderAdd(&(response->header),
					new(HttpHeader, CSTRA("Content-Length"), buffer, nbuf));
	}

	t    = time(NULL);
	tmp  = localtime(&t);
	nbuf = strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T %Z", tmp);
	httpHeaderAdd(&(response->header),
			new(HttpHeader, CSTRA("Date"), buffer, nbuf));
}

// vim: set ts=4 sw=4:
