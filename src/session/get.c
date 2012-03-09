#include <search.h>
#include <time.h>

#include "session.h"


static
inline
int
sessionGetComp(const void * _a, const void * _b)
{
	unsigned long a = *(unsigned long *)_a;
	Session       b = (Session)_b;
	return (a < b->id)? -1 : (a > b->id)? 1 : 0;
}

Session
sessionGet(const Session * root, const unsigned long id)
{
	Session * found = tfind(&id, (void**)root, sessionGetComp);

	if (NULL == found) {
		return NULL;
	}

	return *found;
}

// vim: set ts=4 sw=4:
