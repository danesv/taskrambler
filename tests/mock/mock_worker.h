#ifndef __MOCK_MOCK_WORKER_H__
#define __MOCK_MOCK_WORKER_H__
#endif // __MOCK_MOCK_WORKER_H__

#include "class.h"

CLASS(MockWorker) {
	char _rbuf[1024];
	char _wbuf[1024];
	char * rbuf;
	char * wbuf;
};

// vim: set ts=4 sw=4:
