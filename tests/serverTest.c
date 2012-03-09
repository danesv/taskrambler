#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <socket.h>

#include "runtest.h"
#include "logger.h"
#include "cclass.h"
#include "server.h"
#include "signalHandling.h"


#define TEST_PORT	11212
#define TEST_DATA	"test"


int  level  = -1;
char msg[1024];
char buffer[1025];

static void
read_hook(const char * _buffer, size_t size)
{
	memset(buffer, 0, 1025);

	if (NULL != _buffer) {
		strncpy(buffer, _buffer, 1024>size? size : 1024);
	}

	doShutdown = 1;
}

static void
logfnct_mock(int _level, const char * _msg)
{
    level = _level;
    strncpy(msg, _msg, 1023);
}

const char testname[] = "serverTest";
LOGGER logger = NULL;
SERVER server = NULL;

static
int
__setUp()
{
	logger = new(LOGGER, NULL);
	logger_add(logger, logfnct_mock);

	server = new(SERVER, logger, TEST_PORT, SOMAXCONN);

	ASSERT_INSTANCE_OF(SERVER, server);
	ASSERT_INSTANCE_OF(LOGGER, server->logger);
	ASSERT_INSTANCE_OF(SOCK, server->sock);
	ASSERT_EQUAL(TEST_PORT, server->sock->port);

	server->read_hook = read_hook;

	return TEST_OK;
}
int (* const setUp)() = __setUp;

static
int
__tearDown()
{
    level = -1;

    if (NULL != server) {
        ASSERT_OBJECT(server);
        delete(&server);
    }

    if (NULL != logger) {
        ASSERT_OBJECT(logger);
        delete(&logger);
    }

    return TEST_OK;
}
int (* const tearDown)() = __tearDown;

static
int
testDummy()
{
	SOCK con;
	pid_t pid;

	pid = fork();

	switch(pid) {
		case 0:
			con = new(SOCK, logger, TEST_PORT);
			sleep(1);
			sock_connect(con, "127.0.0.1");
			write(con->handle, TEST_DATA, strlen(TEST_DATA)+1);
			delete(&con);
			__tearDown();
			exit(EXIT_SUCCESS);

		case -1:
			return TEST_FAILED;

		default:
			init_signals();
			server_run(server);
	}

	ASSERT_STRING_EQUAL(TEST_DATA, buffer);

	return TEST_OK;
}

const testfunc tests[] = {
    testDummy
};
const size_t count = FUNCS_COUNT(tests);

// vim: set ts=4 sw=4:
