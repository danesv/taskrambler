#include <search.h>

#include "session.h"
#include "interface/class.h"


static
inline
int
sessionAddComp(const void * _a, const void * _b)
{
	Session a = (Session)_a;
	Session b = (Session)_b;
	return (a->id < b->id)? -1 : (a->id > b->id)? 1 : 0;
}

Session
sessionAdd(const Session * root, Session session)
{
	Session * found = tsearch(session, (void**)root, sessionAddComp);

	if (NULL == found) {
		return NULL;
	}

	if (*found != session) {
		/**
		 * \todo this should not happen, so do some logging here.
		 */
		delete(session);
	}

	return *found;
}

// vim: set ts=4 sw=4:
