#include <search.h>

#include "session.h"
#include "interface/class.h"


static
inline
int
sessionDeleteComp(const void * _a, const void * _b)
{
	unsigned long a = *(unsigned long *)_a;
	Session       b = (Session)_b;
	return (a < b->id)? -1 : (a > b->id)? 1 : 0;
}

void
sessionDelete(const Session * root, const unsigned long id)
{
	tdelete(&id, (void**)root, sessionDeleteComp);
}

// vim: set ts=4 sw=4:
