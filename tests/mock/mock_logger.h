#ifndef __MOCK_MOCK_LOGGER_H__
#define __MOCK_MOCK_LOGGER_H__

#define MOCK_MESSAGE_MAX	1024

#include "class.h"
#include "logger.h"

CLASS(MockLogger)
{
	EXTENDS(Logger);

	char message[MOCK_MESSAGE_MAX];
};

void mockLoggerCleanMsg(MockLogger);

#endif // __MOCK_MOCK_LOGGER_H__

// vim: set ts=4 sw=4:
