#include <stdlib.h>

#include "utils/memory.h"

void
ffree(void ** data)
{
	if (NULL != *data) {
		free(*data);
		*data = NULL;
	}
}   

// vim: set ts=4 sw=4:
