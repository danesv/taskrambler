#include <stdio.h>

#include "class.h"
#include "logger.h"
#include "mock/mock_logger.h"

static
void
logMock(void * _this, logger_level level, const char * const msg)
{
	MockLogger this = _this;

	snprintf(this->message,
			MOCK_MESSAGE_MAX - 1,
			"[%s] %s",
			logger_level_str[level],
			msg);
}

void
mockLoggerCleanMsg(MockLogger this)
{
	this->message[0] = 0;
}

INIT_IFACE(Logger, logMock);
CREATE_CLASS(MockLogger, Logger, IFACE(Logger));

// vim: set ts=4 sw=4:
